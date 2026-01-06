#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    const char* key;
    int value;
    struct ht_item* next;
} ht_item;

typedef struct {
    int size;
    int count;
    ht_item** items;
} hash_table;

int search(hash_table* ht, const char *key);

int hash(const char* key) {
    int pos;
    int first_letter = tolower(key[0]);
    if(first_letter >= 'a' && first_letter <= 'z')
    {
        pos = first_letter - 'a';
    }
    else
    {
        pos = 26;
    }
    return pos;
}

struct ht_item *add_last(struct ht_item* list, struct ht_item *newd)
{
    if (!list)
        return newd;

    struct ht_item *result = list;
    while (result->next)
        result = result->next;
    result->next = newd;
    return list;
}

void insert(hash_table* ht, const char *key, int value) {
    ht_item* item;
    item = (ht_item *) malloc(sizeof(ht_item));
    item->key = key;
    item->value = value;

    int pos = hash(key);
    add_last(&ht[pos], &item);
}
void reseize(hash_table* ht);
char** keys(hash_table* ht);

int main(int argc, char *argv[]) {
    hash_table ht;

    insert(&ht, "key1", 1);
    insert(&ht, "key2", 2);
    insert(&ht, "key3", 3);

    printf("%d\n", search(&ht, "key1"));
    printf("%d\n", search(&ht, "key2"));
    printf("%d\n", search(&ht, "key3"));

    return EXIT_SUCCESS;
}
