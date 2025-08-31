#ifndef STRUCT_H
#define STRUCT_H
#include <stddef.h>

typedef enum {
    BREAD,
    TOMATO,
    LETTUCE,
    CHEESE,
    MEAT,
} ingredient_t;


typedef struct order{
    ingredient_t *ingredients;
    size_t count;
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


} band_t;

#endif



