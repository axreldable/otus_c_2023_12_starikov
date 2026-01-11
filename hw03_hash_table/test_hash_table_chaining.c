#include <stdio.h>
#include "hash_table_chaining.h"
#include "../simple_unit.h"


int test_ht(void) {
    hash_table *ht = ht_new();

    for (int i = 0; i < 20; i++) {
        char *key = malloc(20);
        sprintf(key, "key%d", i);
        insert(ht, key, i);
        free(key);
    }

    ASSERT_EQ(1, search(ht, "key1"));
    ASSERT_EQ(11, search(ht, "key11"));
    ASSERT_EQ(13, search(ht, "key13"));

    insert(ht, "key1", 2);
    insert(ht, "key11", 3);
    insert(ht, "key13", 4);
    ASSERT_EQ(2, search(ht, "key1"));
    ASSERT_EQ(3, search(ht, "key11"));
    ASSERT_EQ(4, search(ht, "key13"));

    return 0;
}

int test_text(void) {
    char *in_file_p = "text.txt";
    FILE *fp = fopen(in_file_p, "rb");
    if (!fp) {
        fprintf(stderr, "Can't open input file %s\n", in_file_p);
        return EXIT_FAILURE;
    }

    hash_table *ht = ht_new();
    char word[100];
    while (fscanf(fp, "%99s", word) == 1) {
        // printf("%s\n", word);
        const int count = search(ht, word);
        if (count == -1) {
            insert(ht, word, 1);
        } else {
            insert(ht, word, count + 1);
        }
    }
    ASSERT_EQ(6, search(ht, "wisdom"));
    ASSERT_EQ(3, search(ht, "river"));
    ASSERT_EQ(1, search(ht, "lightning"));
    ASSERT_EQ(2, search(ht, "snow"));
    ASSERT_EQ(3, search(ht, "alpha"));

    ASSERT_EQ(-1, search(ht, "unknown"));

    fclose(fp);

    display_ht(ht);
    return EXIT_SUCCESS;
}

void manual_tests(void) {
    hash_table *ht = ht_new();

    for (int i = 0; i < 20; i++) {
        char *key = malloc(20);
        sprintf(key, "key%d", i);
        insert(ht, key, i);
        free(key);
    }
    insert(ht, "key1", 2);

    display_ht(ht);

    printf("%d\n", search(ht, "key1"));
    printf("%d\n", search(ht, "key11"));
    printf("%d\n", search(ht, "key13"));
}


int main(int argc, char *argv[]) {
    manual_tests();

    EXECUTE_TESTS(
        test_ht,
        test_text
    );
}
