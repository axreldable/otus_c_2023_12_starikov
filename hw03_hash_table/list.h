#pragma once

#include <stdio.h>
#include <stdlib.h>

struct NODE {
    int data;
    struct NODE *next;
};

struct NODE *add_first(struct NODE *list, int value);
struct NODE *add_last(struct NODE *list, int value);
struct NODE *find(const struct NODE *list, int value);
struct NODE *delete_node(struct NODE *list, int value);
void display_list(const struct NODE *head);
int size(const struct NODE *head);
void free_list(struct NODE *list);


struct NODE *create_node(int value) {
    struct NODE *node = calloc(1, sizeof(struct NODE));
    if (!node) {
        fprintf(stderr, "Out of memory while creating node\n");
        exit(EXIT_FAILURE);
    }
    node->data = value;
    return node;
}

inline struct NODE *add_first(struct NODE *list, int value) {
    struct NODE *new_node = create_node(value);
    new_node->next = list;
    return new_node;
}

inline struct NODE *add_last(struct NODE *list, int value) {
    struct NODE *new_node = create_node(value);
    if (!list) {
        return new_node;
    }

    struct NODE *ptr = list;
    while (ptr->next) {
        ptr = ptr->next;
    }
    ptr->next = new_node;
    return list;
}

inline struct NODE *find(const struct NODE *list, int value) {
    const struct NODE *ptr = list;
    while (ptr != NULL) {
        if (ptr->data == value) {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

inline struct NODE *delete_node(struct NODE *list, int value) {
    if (!list) {
        return NULL;
    }

    struct NODE *ptr = list;
    if (ptr->data == value) {
        struct NODE *rez = ptr->next;
        free(ptr);
        return rez;
    }

    while (ptr->next != NULL) {
        if (ptr->next->data == value) {
            struct NODE *del = ptr->next;
            ptr->next = ptr->next->next;
            free(del);
            return list;
        }
        ptr = ptr->next;
    }
    return list;
}

inline void display_list(const struct NODE *head) {
    const struct NODE *ptr = head;
    printf("List: ");
    while (ptr != NULL) {
        printf("%d ", ptr->data);
        ptr = ptr->next;
    }
    printf("\n");
}

inline int size(const struct NODE *head) {
    const struct NODE *ptr = head;
    int count = 0;
    while (ptr) {
        ptr = ptr->next;
        count++;
    }
    return count;
}

inline void free_list(struct NODE *list) {
    struct NODE *current = list;
    while (current != NULL) {
        struct NODE *next = current->next;
        free(current);
        current = next;
    }
}
