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

typedef struct band {
    int bread_ingredients;
    int tomato_ingredients;
    int lettuce_ingredients;
    int cheese_ingredients;
    int meat_ingredients;
    size_t produced;
} band_t;

#endif



