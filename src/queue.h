/**
 * @brief Queue used for construction of Hufmann tree
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include "huffman.h"

struct node_queue {
    Node **data;
    int first;
    int last;
    int capacity;
};

typedef struct node_queue queue_t;

/**
 * @brief initialize queue
 */
queue_t *queue_init(queue_t *q, Node **data, int capacity, int first, int last);

/**
 * @brief get nr of elements currently in queue
 */
int queue_size(queue_t *q);

/**
 * @brief returns true if queue is empty
 */
bool queue_empty(queue_t *q);

/**
 * @brief get total capacity of queue
 */
int queue_capacity(queue_t *q);

/**
 * @brief add 'node' at the end of queue 'q'
 */
void queue_enqueue(queue_t *q, Node *node);

/**
 * @brief remove first element from queue and returns it
 */
Node *queue_dequeue(queue_t *q);

/**
 * @brief get first element of queue (without removing it)
 */
Node *queue_peek(queue_t *q);

#endif
