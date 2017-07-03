/**
 * @brief Stack used for Hufmann tree traversal
 */

#ifndef STACK_H
#define STACK_H

#include "huffman.h"

struct stack_entry {
    const Node *node;
    int code;
    int len;
};

struct stack {
    struct stack_entry *data;
    int                size;
    int                capacity;
};

struct stack *stack_init(struct stack *s, struct stack_entry *entries, int capacity);

int stack_size(struct stack *s);

int stack_capacity(struct stack *s);

void stack_push(struct stack *s, const struct stack_entry *entry);

struct stack_entry *stack_pop(struct stack *s);
 
#endif
