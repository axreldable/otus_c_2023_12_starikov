#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 10
#define HT_PRIME 151

// https://www.youtube.com/watch?v=kJSkeSMWybY&t=407s
// https://github.com/jamesroutley/write-a-hash-table
typedef struct {
    char* key;
    int value;
    struct ht_item* next;
} ht_item;

typedef struct {
    int size;
    int count;
    ht_item** items;
} hash_table;

inline float load_factor(hash_table* ht) {
    return (float) ht->count / ht->size;
}

inline ht_item* ht_new_item(const char* k, int v) {
    ht_item* i = malloc(sizeof(ht_item));
    i->key = strdup(k);
    i->value = v;
    return i;
}

inline hash_table* ht_new() {
    hash_table* ht = malloc(sizeof(hash_table));

    ht->size = INITIAL_SIZE;
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
    return ht;
}

inline void ht_del_item(ht_item* i) {
    free(i->key);
    free(i);
}


inline void ht_del_hash_table(hash_table* ht) {
    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];
        if (item != NULL) {
            ht_del_item(item);
        }
    }
    free(ht->items);
    free(ht);
}

inline  int ht_hash(const char* s, const int m) {
    int a = HT_PRIME;
    long hash = 0;
    const int len_s = strlen(s);
    for (int i = 0; i < len_s; i++) {
        hash += (long)pow(a, len_s - (i+1)) * s[i];
        hash = hash % m;
    }
    return (int)hash;
}

// todo: implement methods for channing hash_table
// struct ht_item *add_last(struct ht_item* list, struct ht_item *newd)
// {
//     if (!list)
//         return newd;
//
//     struct ht_item *result = list;
//     while (result->next)
//         result = result->next;
//     result->next = newd;
//     return list;
// }
//
// void insert(struct hash_table* ht, const char *key, int value) {
//     ht_item* item = ht_new_item(key, value);
//     int pos = hash(key);
//     add_last(&ht[pos], &item);
// }

int search(hash_table* ht, const char *key);

void reseize(hash_table* ht);
char** keys(hash_table* ht);
