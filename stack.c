#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

struct stack *stack_init(int capacity)
{
    struct stack *s = malloc(sizeof(struct stack));
    s->data = malloc(sizeof(struct stack_entry) * capacity);
    s->size = 0;
    s->capacity = capacity;
    return s;
}

int stack_size(struct stack *s)
{
    return s->size;
}

int stack_capacity(struct stack *s)
{
    return s->capacity;
}

void stack_push(struct stack *s, struct stack_entry *entry)
{
    if (s->size == s->capacity) {
        fprintf(stderr, "Error: trying to push onto full stack\n");
        return;
    }

    s->data[s->size] = *entry;
    s->size++;
}


struct stack_entry *stack_pop(struct stack *s)
{
    if (s->size == 0){
        fprintf(stderr, "Error: trying to pop from empty stack\n");
        return NULL;
    }

    s->size--;
    return &s->data[s->size];
}
