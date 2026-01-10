#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 10
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


static void insert(hash_table *ht, const char *key, int value) {
    ht_item *item = ht_new_item(key, value);
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

int search(hash_table *ht, const char *key) {
    int pos = ht_hash(key, ht->size);
    ht_item *ptr = ht->items[pos];
    while (ptr != NULL) {
        if (strcmp(ptr->key, key) == 0) {
            return ptr->value;
        }
        ptr = ptr->next;
    }
    return -1;
}

void reseize(hash_table *ht);

char **keys(hash_table *ht);
