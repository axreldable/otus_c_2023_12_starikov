#include <stdio.h>
#include "list.h"
#include "../simple_unit.h"


int test_list(void) {
    struct NODE *list = NULL;
    list = add_first(list, 2);
    list = add_first(list, 1);
    list = add_last(list, 3);

    ASSERT_EQ(1, find(list, 1)->data);
    ASSERT_EQ(2, find(list, 2)->data);
    ASSERT_EQ(3, find(list, 3)->data);

    return 0;
}

int test_list_delete(void) {
    struct NODE *list = NULL;
    list = add_first(list, 2);
    list = add_first(list, 1);
    list = add_last(list, 3);

    ASSERT_EQ(3, size(list));

    list = delete_node(list, 2);
    ASSERT_EQ(2, size(list));

    list = delete_node(list, 1);
    ASSERT_EQ(1, size(list));

    list = delete_node(list, 3);
    ASSERT_EQ(0, size(list));

    list = delete_node(list, 3);
    ASSERT_EQ(0, size(list));

    return 0;
}

void manual_tests(void) {
    struct NODE *list = NULL;

    display_list(list);

    list = add_first(list, 2);
    list = add_first(list, 1);
    list = add_last(list, 3);

    display_list(list);

    struct NODE *rez = find(list, 2);
    display_list(rez);

    list = delete_node(list, 2);
    display_list(list);

    free_list(list);
}


int main(int argc, char *argv[]) {
    manual_tests();

    EXECUTE_TESTS(
        test_list,
        test_list_delete
    );
}
