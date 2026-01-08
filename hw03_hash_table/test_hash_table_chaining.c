#include <stdio.h>
#include "hash_table_chaining.h"
#include "../simple_unit.h"


int test_list(void) {

    return 0;
}

void manual_tests(void) {
    hash_table* ht = ht_new();

    // insert(&ht, "key1", 1);
    // insert(&ht, "key2", 2);
    // insert(&ht, "key3", 3);
    //
    // printf("%d\n", search(&ht, "key1"));
    // printf("%d\n", search(&ht, "key2"));
    // printf("%d\n", search(&ht, "key3"));

    float lf = load_factor(ht);
    printf("load factor = %f\n", lf);
}


int main(int argc, char *argv[]) {
    manual_tests();

    EXECUTE_TESTS(
        test_list
    );
}
