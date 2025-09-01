#ifndef QUEUE_H
#define QUEUE_H
#include "../struct.h"

bool queue_empty(order_queue_t *queue);
int queue_order(order_queue_t *queue, order_t *order);
order_t* dequeue_order(order_queue_t *queue);
order_t* peak(order_queue_t *queue);
bool isEmpty(order_queue_t *queue);

#endif