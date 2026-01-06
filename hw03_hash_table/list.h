#pragma once
#include <stdio.h>
#include <stdlib.h>

struct NODE {
    int data;
    struct NODE *next;
};

struct NODE *create_node(int value) {
    struct NODE *node = calloc(1, sizeof(struct NODE));
    node->data = value;
    return node;
}

struct NODE *add_first(struct NODE *list, int value) {
    struct NODE *new_node = create_node(value);
    new_node->next = list;
    return new_node;
}

struct NODE *add_last(struct NODE *list, int value) {
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

struct NODE *find(struct NODE *list, int value) {
    struct NODE *ptr = list;
    while (ptr != NULL) {
        if (ptr->data == value) {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

struct NODE *delete_node(struct NODE *list, int value) {
    struct NODE *ptr = list;
    while (ptr->next != NULL) {
        if (ptr->next->data == value) {
            ptr->next = ptr->next->next;
            return ptr;
        }
        ptr = ptr->next;
    }
    return list;
}

void display_list(const struct NODE *head) {
    struct NODE *ptr = head;
    printf("List: ");
    while (ptr != NULL) {
        printf("%d ", ptr->data);
        ptr = ptr->next;
    }
    printf("\n");
}

int size(const struct NODE *head) {
    struct NODE *ptr = head;
    int size = 0;
    while (ptr) {
        ptr = ptr->next;
        size++;
    }
    return size;
}

void free_list(struct NODE *list) {
    struct NODE *current = list;
    struct NODE *next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}
