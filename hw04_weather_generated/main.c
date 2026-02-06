/*
 * hw04_weather: fetch current weather from wttr.in API (JSON) and print
 * forecast for the current day.
 * Uses libcurl for HTTP and cJSON for JSON parsing.
 */

#define _POSIX_C_SOURCE 200809L

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"

#define WTTR_BASE "https://wttr.in/"
#define WTTR_FORMAT "?format=j1"
#define BUF_INIT_SIZE 4096u

typedef struct {
    char *data;
    size_t size;
    size_t capacity;
} memory_buffer_t;

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    size_t realsize = size * nmemb;
    memory_buffer_t *buf = (memory_buffer_t *)userdata;
    size_t need = buf->size + realsize + 1u;

    if (need > buf->capacity) {
        size_t new_cap = (buf->capacity == 0u) ? BUF_INIT_SIZE : buf->capacity;
        while (new_cap < need) {
            if (new_cap > (size_t)(-1) / 2u) {
                return 0;
            }
            new_cap *= 2u;
        }
        char *new_data = (char *)realloc(buf->data, new_cap);
        if (new_data == NULL) {
            return 0;
        }
        buf->data = new_data;
        buf->capacity = new_cap;
    }
    memcpy(buf->data + buf->size, ptr, realsize);
    buf->size += realsize;
    buf->data[buf->size] = '\0';
    return realsize;
}

static void buffer_cleanup(memory_buffer_t *buf)
{
    free(buf->data);
    buf->data = NULL;
    buf->size = 0u;
    buf->capacity = 0u;
}

static const char *get_string_safe(const cJSON *obj, const char *key)
{
    const cJSON *item = cJSON_GetObjectItem(obj, key);
    if (item == NULL || !cJSON_IsString(item)) {
        return NULL;
    }
    return item->valuestring;
}

static int fetch_weather(const char *city, memory_buffer_t *buf)
{
    CURL *curl;
    CURLcode res;
    long http_code = 0L;
    int ret = -1;
    size_t city_len;
    char *url = NULL;
    size_t i;
    size_t j;

    if (city == NULL || buf == NULL) {
        return -1;
    }

    /* Build URL: base + city (spaces as +) + format */
    city_len = strlen(city);
    url = (char *)malloc(strlen(WTTR_BASE) + city_len * 3u + strlen(WTTR_FORMAT) + 1u);
    if (url == NULL) {
        fprintf(stderr, "Error: memory allocation failed\n");
        return -1;
    }
    strcpy(url, WTTR_BASE);
    j = strlen(url);
    for (i = 0u; i < city_len; i++) {
        if (city[i] == ' ') {
            url[j++] = '+';
        } else {
            url[j++] = (char)(unsigned char)city[i];
        }
    }
    url[j] = '\0';
    strcat(url, WTTR_FORMAT);

    curl = curl_easy_init();
    if (curl == NULL) {
        fprintf(stderr, "Error: failed to initialize libcurl\n");
        free(url);
        return -1;
    }

    buf->data = NULL;
    buf->size = 0u;
    buf->capacity = 0u;

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, buf);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "hw04_weather/1.0");

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "Error: network request failed: %s\n", curl_easy_strerror(res));
        goto cleanup_curl;
    }

    res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    if (res != CURLE_OK || http_code != 200L) {
        if (http_code == 404L || http_code == 503L) {
            fprintf(stderr, "Error: location not found or service unavailable\n");
        } else {
            fprintf(stderr, "Error: HTTP error %ld\n", http_code);
        }
        goto cleanup_curl;
    }

    if (buf->data == NULL || buf->size == 0u) {
        fprintf(stderr, "Error: empty response from server\n");
        goto cleanup_curl;
    }

    ret = 0;

cleanup_curl:
    curl_easy_cleanup(curl);
    free(url);
    if (ret != 0) {
        buffer_cleanup(buf);
    }
    return ret;
}

static int print_weather(const char *json_str)
{
    cJSON *root = NULL;
    cJSON *current_arr = NULL;
    cJSON *current = NULL;
    cJSON *weather_desc_arr = NULL;
    cJSON *weather_desc_obj = NULL;
    const char *desc = NULL;
    const char *wind_dir = NULL;
    const char *wind_speed = NULL;
    const char *temp_c = NULL;
    int ret = -1;

    root = cJSON_Parse(json_str);
    if (root == NULL) {
        const char *err = cJSON_GetErrorPtr();
        fprintf(stderr, "Error: invalid JSON from server%s\n",
                err ? "" : " (parse error)");
        return -1;
    }

    current_arr = cJSON_GetObjectItem(root, "current_condition");
    if (current_arr == NULL || !cJSON_IsArray(current_arr)) {
        fprintf(stderr, "Error: location not found or invalid response\n");
        goto cleanup;
    }

    current = cJSON_GetArrayItem(current_arr, 0);
    if (current == NULL) {
        fprintf(stderr, "Error: location not found or invalid response\n");
        goto cleanup;
    }

    weather_desc_arr = cJSON_GetObjectItem(current, "weatherDesc");
    if (weather_desc_arr != NULL && cJSON_IsArray(weather_desc_arr)) {
        weather_desc_obj = cJSON_GetArrayItem(weather_desc_arr, 0);
        if (weather_desc_obj != NULL) {
            desc = get_string_safe(weather_desc_obj, "value");
        }
    }

    wind_dir = get_string_safe(current, "winddir16Point");
    wind_speed = get_string_safe(current, "windspeedKmph");
    temp_c = get_string_safe(current, "temp_C");

    printf("Weather (current):\n");
    printf("  Description: %s\n", desc ? desc : "(n/a)");
    printf("  Wind:        %s %s km/h\n",
           wind_dir ? wind_dir : "(n/a)",
           wind_speed ? wind_speed : "(n/a)");
    printf("  Temperature: %s °C\n", temp_c ? temp_c : "(n/a)");

    ret = 0;

cleanup:
    cJSON_Delete(root);
    return ret;
}

int main(int argc, char *argv[])
{
    memory_buffer_t buf = {0};
    int ret;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <city>\n", argv[0] != NULL ? argv[0] : "weather");
        fprintf(stderr, "Example: %s Moscow\n", argv[0] != NULL ? argv[0] : "weather");
        return EXIT_FAILURE;
    }

    ret = fetch_weather(argv[1], &buf);
    if (ret != 0) {
        return EXIT_FAILURE;
    }

    ret = print_weather(buf.data);
    buffer_cleanup(&buf);

    if (ret != 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
