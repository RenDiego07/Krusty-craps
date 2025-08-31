#include "dispenser.h"
#include <stdlib.h>
#include "../struct.h"
#include <stdio.h>


band_t init_band(){
    band_t b;
    b.bread_ingredients = 10;
    b.lettuce_ingredients = 10;
    b.cheese_ingredients = 10;
    b.meat_ingredients = 10;
    b.tomato_ingredients = 10;
    b.produced = 0;
    return b;
}


int cook_hamburger(order_t *order, band_t *band){
    if ( order == NULL) {
        printf("ORDER IS NULL\n");
        return -1;
    }
    if( band == NULL){
        printf("BAND IS NULL\n");
        return -1;
    }
    int tomato = 0;
    int bread = 0;
    int lettuce = 0;
    int meat = 0;
    int cheese = 0;
    
    for ( size_t i = 0; i<order->count; i++){
        ingredient_t ingredient = order->ingredients[i];
        switch ( ingredient) {
            case TOMATO:
                //band->tomato_ingredients = band->tomato_ingredients -1;
                tomato++;
                break;
            case BREAD:
                //band->bread_ingredients = band->bread_ingredients - 1;
                bread++;
                break;
            case LETTUCE:
                //band->lettuce_ingredients = band->lettuce_ingredients -1;
                lettuce++;
                break;
            case MEAT:
                //band->meat_ingredients = band->meat_ingredients -1;
                meat++;
                break;
            case CHEESE:
                //band->cheese_ingredients = band->cheese_ingredients -1;
                cheese++;
                break;
            default:
                printf("COULD NOT DETERMINE WHAT INGREDIENT WAS RECEIVED\n");
                continue;
        }
    }
    if ( tomato > band->tomato_ingredients || 
        bread > band->bread_ingredients || 
        lettuce > band->lettuce_ingredients || 
        meat > band->meat_ingredients || 
        cheese > band->cheese_ingredients){   
            return -1;
        
        }
    band->tomato_ingredients = band->tomato_ingredients - tomato;
    band->bread_ingredients = band->bread_ingredients - bread;
    band->lettuce_ingredients = band->lettuce_ingredients - lettuce;
    band->meat_ingredients =  band->meat_ingredients - meat;
    band->cheese_ingredients = band->cheese_ingredients - cheese;
    band->produced++;
        
    return 0;
}

int band_status (band_t *band){
    if( band == NULL){
        return -1;
    }
    printf("NUMBER OF BREAD LEFT :  %d\n", band->bread_ingredients);
    printf("NUMBER OF TOMATO LEFT : %d\n", band->tomato_ingredients);
    printf("NUMBER OF LETTUCE LEFT : %d\n", band->lettuce_ingredients);
    printf("NUMBER OF MEAT LEFT : %d\n", band->meat_ingredients);
    printf("NUMBER OF CHEESE LEFT : %d\n", band->cheese_ingredients);
    printf("HAMBURGERS MADE %zu\n", band->produced);
    return 0;
}


band_t *initialize_bands(int n){
    if(n < 0){
        return NULL;
    }
    band_t *bands = (band_t *) malloc(sizeof(band_t) * n);
    if( bands == NULL ){
        return NULL;
    }
    for( int i = 0; i < n; i++){
        bands[i] = init_band();
    }
    return bands; 
}