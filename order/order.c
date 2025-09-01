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

order_data_t *get_info(order_t *order){
      order_data_t *data = (order_data_t *)malloc(sizeof(order_data_t));
      if(data == NULL) {
          printf("COULD NOT ALLOCATE MEMORY FOR ORDER DATA\n");
          return NULL;
      }

      if(order != NULL) {
          data->num_bread = 0;
          data->num_tomato = 0;
          data->num_lettuce = 0;
          data->num_cheese = 0;
          data->num_meat = 0;

          for(size_t i = 0; i < order->count; i++){
              switch(order->ingredients[i]) {
                  case BREAD:
                      data->num_bread++;
                      break;
                  case TOMATO:
                      data->num_tomato++;
                      break;
                  case LETTUCE:
                      data->num_lettuce++;
                      break;
                  case CHEESE:
                      data->num_cheese++;
                      break;
                  case MEAT:
                      data->num_meat++;
                      break;
                  default:
                      // Unknown ingredient, skip
                      break;
              }
          }

          return data;
      } else {
          printf("ORDER IS NULL\n");
          free(data);
          return NULL;
      }
  }






