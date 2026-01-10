#include <stdio.h>
#include "hash_table_chaining.h"
#include "../simple_unit.h"


int test_list(void) {

    return 0;
}

void manual_tests(void) {
    hash_table* ht = ht_new();

    for (int i = 0; i < 20; i++) {
        char *key = malloc(20);
        sprintf(key, "key%d", i);
        insert(ht, key, i);
        free(key);
    }

    display_ht(ht);

    printf("%d\n", search(ht, "key1"));
    printf("%d\n", search(ht, "key11"));
    printf("%d\n", search(ht, "key13"));
}


int main(int argc, char *argv[]) {
    manual_tests();

    EXECUTE_TESTS(
        test_list
    );
}
