#ifndef STRUCT_H
#define STRUCT_H
#include <stddef.h>
#include <pthread.h>

typedef enum {
    BREAD,
    TOMATO,
    LETTUCE,
    CHEESE,
    MEAT,
} ingredient_t;

typedef struct order_data {
    size_t num_bread;
    size_t num_tomato;
    size_t num_lettuce;
    size_t num_cheese;
    size_t num_meat;
} order_data_t;

typedef struct order{
    ingredient_t *ingredients;
    size_t count;
    size_t id;
    
} order_t;

typedef struct queue_node {
    order_t *order;
    struct queue_node *next;
} queue_node_t;


typedef struct order_queue {
    queue_node_t *front;
    queue_node_t *last;
    size_t size;
} order_queue_t;


typedef struct band {
    int bread_ingredients;
    int tomato_ingredients;
    int lettuce_ingredients;
    int cheese_ingredients;
    int meat_ingredients;
    size_t produced;
    
    order_queue_t order_queue;
    order_queue_t waiting_queue; 

    pthread_mutex_t waiting_queue_mutex;
    pthread_mutex_t queue_mutex;
    pthread_cond_t queue_condition;

    size_t orders_missed;     
    size_t id;

} band_t;

typedef struct bands_data {
    band_t *bands;
    size_t size;
} bands_data_t;

#endif



