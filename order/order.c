#include "order.h"
#include <stdio.h>
#include <stdlib.h>

order_t *init_order(char *stream, size_t length){
    order_t *order = (order_t *) malloc(sizeof(order_t));
    if(order == NULL){
        printf("COULD NOT ALLOCATE MORE MEMORY\n");
        return NULL;    
    }
    
    // Allocate memory for ingredients array
    order->ingredients = (ingredient_t *) malloc(length * sizeof(ingredient_t));
    if(order->ingredients == NULL){
        printf("COULD NOT ALLOCATE MEMORY FOR INGREDIENTS\n");
        free(order);
        return NULL;
    }
    
    order->count = 0;
    
    for(size_t i = 0; i < length; i++){
        char ingredient = stream[i];
        switch(ingredient){
            case 'B': // B stands for bread
                // printf("BREAD DETECTED\n");
                order->ingredients[order->count++] = BREAD;
                break;
            case 'L': // L stands for lettuce;
                // printf("LETTUCE DETECTED\n");
                order->ingredients[order->count++] = LETTUCE;
                break;
            case 'C': // C stands for cheese
                // printf("CHEESE DETECTED \n");
                order->ingredients[order->count++] = CHEESE;
                break;
            case 'M': // M stands for meat;
                // printf("MEET DETECTED \n");
                order->ingredients[order->count++] = MEAT;
                break;
            case 'T': // T stands for TOMATO
                // printf("TOMATO DETECTED \n");
                order->ingredients[order->count++] = TOMATO;
                break;
            default:
                //Skip unknown characters
                break;
        }
    }
    
    return order;
}