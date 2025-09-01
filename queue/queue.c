#include <stdbool.h>
#include "../struct.h"
#include "./queue.h"
#include <stdlib.h>
#include <stdio.h>


bool queue_empty(order_queue_t *queue){
    if(queue->size == 0){
        return true;
    }
    return false;
}

int queue_order(order_queue_t *queue, order_t *order){
    if( order == NULL) {
        printf("ORDER IS NULL\n");
        return -1;
    }

    if( queue == NULL ){
        printf("QUEUE IS NULL\n");
        return -1;
    }
    queue_node_t *new_node = (queue_node_t *) malloc(sizeof(queue_node_t));
    new_node->next = NULL;
    new_node->order = order;

    if( queue->size == 0){
        queue->front = new_node;
        queue->last = new_node;
    }else if(queue->size == 1){
        queue->front->next = new_node;
        queue->last = new_node;
    }else{
        queue->last->next = new_node;
        queue->last = new_node;
    }
    queue->size++;
    return 1;
}


order_t* dequeue_order(order_queue_t *queue){
    if ( queue == NULL){
        printf("QUEUE IS NULL\n");
        return NULL;
    }
    if( queue->size == 0 || queue->front == NULL){
        return NULL;
    }
    order_t *order = NULL;
    queue_node_t *node = queue->front;
    order = node->order;
    queue->front = queue->front->next;
    if(queue->front == NULL){
        queue->last = NULL;
    }    
    queue->size = queue->size -1;
    free(node);
    return order;
}

order_t* peak(order_queue_t *queue){
    if( queue == NULL){
        printf("QUEUE IS NULL\n");
        return NULL;
    }
    if(queue->size == 0){
        printf("THERE AREN'T ANY ORDERS IN THE WAITING QUEUE\n");
        return NULL;
    }
    return queue->front->order;

}

bool isEmpty(order_queue_t *queue){
    if( queue == NULL || queue->size == 0 ){
        return true;
    }
    return false;
}
