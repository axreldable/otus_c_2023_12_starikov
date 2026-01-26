#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 13
#define HT_PRIME 151

// https://www.youtube.com/watch?v=kJSkeSMWybY&t=407s
// https://github.com/jamesroutley/write-a-hash-table
typedef struct ht_item {
    char *key;
    int value;
    struct ht_item *next;
} ht_item;

typedef struct {
    int size;
    int count;
    ht_item **items;
} hash_table;

void insert(hash_table *ht, const char *key, int value);

/**
 * load_factor = Number of occupied slots / length of the hash table
 */
static float load_factor(hash_table *ht) {
    return (float) ht->count / ht->size;
}

static ht_item *ht_new_item(const char *k, int v) {
    ht_item *i = malloc(sizeof(ht_item));
    i->key = strdup(k);
    i->value = v;
    return i;
}

static hash_table *ht_new() {
    hash_table *ht = malloc(sizeof(hash_table));

    ht->size = INITIAL_SIZE;
    ht->count = 0;
    ht->items = calloc((size_t) ht->size, sizeof(ht_item *));
    return ht;
}

static void ht_del_item(ht_item *i) {
    free(i->key);
    free(i);
}


static void ht_del_hash_table(hash_table *ht) {
    for (int i = 0; i < ht->size; i++) {
        ht_item *item = ht->items[i];
        if (item != NULL) {
            ht_del_item(item);
        }
    }
    free(ht->items);
    free(ht);
}

static int ht_hash(const char *s, const int m) {
    int a = HT_PRIME;
    long hash = 0;
    const int len_s = strlen(s);
    for (int i = 0; i < len_s; i++) {
        hash += (long) pow(a, len_s - (i + 1)) * s[i];
        hash = hash % m;
    }
    return (int) hash;
}

static ht_item *add_last(ht_item *list, ht_item *newd) {
    if (!list)
        return newd;

    ht_item *ptr = list;
    while (ptr->next)
        ptr = ptr->next;
    ptr->next = newd;
    return list;
}

static ht_item *_get_item(hash_table *ht, const char *key) {
    int pos = ht_hash(key, ht->size);
    ht_item *ptr = ht->items[pos];
    while (ptr != NULL) {
        if (strcmp(ptr->key, key) == 0) {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

static int search(hash_table *ht, const char *key) {
    ht_item *item = _get_item(ht, key);
    if (item == NULL) {
        return -1;
    }
    return item->value;
}

int is_prime(const int x) {
    if (x < 2) { return -1; }
    if (x < 4) { return 1; }
    if ((x % 2) == 0) { return 0; }
    for (int i = 3; i <= floor(sqrt((double) x)); i += 2) {
        if ((x % i) == 0) {
            return 0;
        }
    }
    return 1;
}

int next_prime(int x) {
    while (is_prime(x) != 1) {
        x++;
    }
    return x;
}

static void resize_up(hash_table *ht) {
    printf("resize_up\n");
    hash_table *new_ht = malloc(sizeof(hash_table));

    int next_size = next_prime(ht->size * 2);
    printf("current_size: %d, next_size: %d", ht->size, next_size);

    new_ht->size = next_size;
    new_ht->count = 0;
    new_ht->items = calloc((size_t) ht->size, sizeof(ht_item *));

    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];
        while (item != NULL) {
            insert(new_ht, item->key, item->value);
            item = item->next;
        }
    }

    const int tmp_size = ht->size;
    ht->size = new_ht->size;
    new_ht->size = tmp_size;

    ht_item** tmp_items = ht->items;
    ht->items = new_ht->items;
    new_ht->items = tmp_items;

    ht_del_hash_table(new_ht);
}


void insert(hash_table *ht, const char *key, int value) {
    const float load = load_factor(ht);
    // printf("load_factor: %f\n", load);
    if (load > 3) {
        resize_up(ht);
    }

    // if exists, update value
    ht_item *item = _get_item(ht, key);
    if (item != NULL) {
        item->value = value;
        return;
    }

    item = ht_new_item(key, value);
    int pos = ht_hash(key, ht->size);
    ht->items[pos] = add_last(ht->items[pos], item);
    ht->count++;
}

static void display_ht(hash_table *ht) {
    printf("Hash table (chaining):\n");
    printf("Number of items: %d\n", ht->count);
    printf("Size of the ht: %d\n", ht->size);
    for (int i = 0; i < ht->size; i++) {
        printf("list[%d]", i);
        ht_item *ptr = ht->items[i];
        while (ptr != NULL) {
            printf("(%s-%d) ", ptr->key, ptr->value);
            ptr = ptr->next;
        }
        printf("\n");
    }
}

char **keys(hash_table *ht);
