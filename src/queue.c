#include <stdlib.h>
#include "queue.h"


queue_t *queue_init(queue_t *q, Node **data, int capacity, int first, int last)
{
    //queue_t *q = malloc(sizeof(queue_t));
    //q->data = malloc(sizeof(Node *) * capacity);
    q->data = data;
    q->first = first;
    q->last = last;
    q->capacity = capacity;
    return q;
}

int queue_size(queue_t *q)
{
    return q->last - q->first + 1;
}

bool queue_empty(queue_t *q)
{
    return queue_size(q)==0 ? true : false;
}

bool queue_full(queue_t *q)
{
    return q->last == q->capacity-1 ? true : false;
}

int queue_capacity(queue_t *q)
{
    return q->capacity;
}

void queue_enqueue(queue_t *q, Node *node)
{
    if (queue_full(q)) {
        return;
    }
    q->data[++q->last] = node;
}

Node *queue_dequeue(queue_t *q)
{
    if (queue_empty(q)) {
        return NULL;
    }
    return q->data[q->first++];
}

Node *queue_peek(queue_t *q)
{
    return q->data[q->first];
}
