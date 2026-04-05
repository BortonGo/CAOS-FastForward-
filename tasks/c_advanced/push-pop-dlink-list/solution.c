#include <stdlib.h>

#include "dlist.h"

Node *push(Node *place, int value) {
    Node *new_node = (Node *)calloc(1, sizeof(*new_node));
    if (new_node == NULL) {
        return NULL;
    }

    new_node->value = value;
    new_node->next = place;

    if (place != NULL) {
        new_node->prev = place->prev;

        if (place->prev != NULL) {
            place->prev->next = new_node;
        }

        place->prev = new_node;
    }

    return new_node;
}

int pop(Node *node) {
    int value = node->value;

    if (node->prev != NULL) {
        node->prev->next = node->next;
    }

    if (node->next != NULL) {
        node->next->prev = node->prev;
    }

    free(node);
    return value;
}