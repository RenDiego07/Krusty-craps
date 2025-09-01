
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#include "./dispenser/dispenser.h"
#include "./order/order.h"
#include "./queue/queue.h"
#include "./struct.h"



int N;
bool KRUSTY_OPEN = true;
char *file;
bool ALL_ORDERS_SERVED = false;
int NUM_ORDER = 0;
size_t TIMES_UNTIL_REFILL = 5;
band_t *global_bands; // Global reference for cross-band access

void* show_band_states(void *arg);
void* band_handler(void* arg);

int main (int argc, char *argv[]){
    if ( argc < 2 ){
        printf("DID NOT PASS ENOUGH ARGUMENTS\n");
        return -1;
    }
    N = atoi(argv[1]);

    // char *bigMc = "BMMMLTB";
    // order_t *order = init_order(bigMc, strlen(bigMc));
     file = argv[2];
    
    FILE *fp = fopen(file, "r");  
    if( fp == NULL ){
        printf("COULD NOT OPEN FILE DESCRIPTOR\n");
        return -1;
    }

    band_t *bands = initialize_bands(N);
    bands_data_t band_data = {bands, N};
    pthread_t thread_status;
    if(pthread_create(&thread_status, NULL, show_band_states, &band_data) !=0){
        printf("COULD NOT INITILIAZE BAND MONITORING THREAD\n");
        return -1;
    }
    pthread_t tid [N];

    for(int i = 0; i < N; i++){
        if(pthread_create(&tid[i], NULL, band_handler, &bands[i]) != 0){
            printf("COULD NOT INITIALIZE THREADS FOR BAND %d\n", i);
            return -1;
        }
        //band_status(&bands[i]);
        printf("\n");
    }
    char request[10];
    int band_id = 0;
    size_t times = 0;

    while(fgets(request, sizeof(request), fp) || ALL_ORDERS_SERVED){
        request[strcspn(request, "\n")] = '\0';
        if( request[0] == '\0'){
            continue;
        }
        order_t *order = init_order(request,strlen(request));
        sleep(2);
        order->id = times;
        band_id = band_id % N;
        //printf("ORDER %s corresponds to the band ID : %d\n", request, band_id);
        
        pthread_mutex_lock(&bands[band_id].queue_mutex);
        queue_order(&bands[band_id].order_queue, order);
        pthread_cond_signal(&bands[band_id].queue_condition);
        pthread_mutex_unlock(&bands[band_id].queue_mutex);
        band_id++;
        times = times + 1;
    }
    fclose(fp);

    for(int i = 0; i < N; i++) {
      pthread_cond_broadcast(&bands[i].queue_condition);
    }
    KRUSTY_OPEN = true;
    for(int i = 0; i < N; i++) {
      pthread_join(tid[i], NULL);  
    }
    clean_bands(bands, N);
    pthread_join(thread_status,NULL);
    return 0;
}

void* band_handler( void* arg){
    band_t *band = (band_t *) arg;
    while( KRUSTY_OPEN ){
        order_t *order = NULL;
        pthread_mutex_lock(&band->waiting_queue_mutex);
        if( !isEmpty(&band->waiting_queue)){
            for (;;) {
                order = peak(&band->waiting_queue);
                if( order != NULL ){
                    printf("CHECKING IF ORDER %zu CAN BE DISPATCHED \n", order->id);
                    printf("THESE ARE THE VALUES FROM THE ORDER THAT WAS PULLED FROM THE WAITING QUEUE\n");
                    order_data_t *order_info = get_info(order);
                    printf("Order %zu needs: B:%zu T:%zu L:%zu C:%zu M:%zu\n",
                            order->id, order_info->num_bread, order_info->num_tomato,
                            order_info->num_lettuce, order_info->num_cheese,
                            order_info->num_meat);
                    
                    printf("Band %zu has: B:%d T:%d L:%d C:%d M:%d\n",
                            band->id, band->bread_ingredients, band->tomato_ingredients,
                            band->lettuce_ingredients, band->cheese_ingredients, band->meat_ingredients);
                    
                    printf("COMPARISON: ");
                    bool can_fulfill = true;
                    if(order_info->num_bread > band->bread_ingredients) {
                        printf("BREAD INSUFFICIENT (%zu needed, %d have) ", order_info->num_bread, band->bread_ingredients);
                        can_fulfill = false;
                    }
                    if(order_info->num_tomato > band->tomato_ingredients) {
                        printf("TOMATO INSUFFICIENT (%zu needed, %d have) ", order_info->num_tomato, band->tomato_ingredients);
                        can_fulfill = false;
                    }
                    if(order_info->num_lettuce > band->lettuce_ingredients) {
                        printf("LETTUCE INSUFFICIENT (%zu needed, %d have) ", order_info->num_lettuce, band->lettuce_ingredients);
                        can_fulfill = false;
                    }
                    if(order_info->num_cheese > band->cheese_ingredients) {
                        printf("CHEESE INSUFFICIENT (%zu needed, %d have) ", order_info->num_cheese, band->cheese_ingredients);
                        can_fulfill = false;
                    }
                    if(order_info->num_meat > band->meat_ingredients) {
                        printf("MEAT INSUFFICIENT (%zu needed, %d have) ", order_info->num_meat, band->meat_ingredients);
                        can_fulfill = false;
                    }
                    printf("\n");
                    free(order_info); // Free the allocated memory
                    
                    if(can_fulfill){
                        cook_hamburger(order, band);
                        printf("ORDER THAT WAS IN QUEUE FINALLY WAS DISPATCHED [ ORDER ID : %zu ] \n", order->id);
                        dequeue_order(&band->waiting_queue);
                    }else{
                        band->orders_missed++;                        
                        printf("DID NOT HAVE ENOUGH INREGREDIENTS TO DISPATCH [ ORDER ID : %zu ] \n", order->id);
                        printf("ORDERS LEFT UNTIL REFILLED %zu\n", TIMES_UNTIL_REFILL - band->orders_missed);
                        if(band->orders_missed >= TIMES_UNTIL_REFILL){
                            printf("******************* band %zu *******************\n", band->id);
                            fill_dispenser(band);
                            band->orders_missed = 0;
                        }
                        break;
                    }
                }else{
                    printf(" HEY, SOMETHING WENT WRONG WITH THE WAITNG QUEUE \n");
                    break;
                }
                sleep(1);
            }
        }
        pthread_mutex_unlock(&band->waiting_queue_mutex);
        pthread_mutex_lock(&band->queue_mutex);
        while(queue_empty(&band->order_queue) && KRUSTY_OPEN){

            // Create timeout: current time + 10 seconds
            struct timespec timeout;
            clock_gettime(CLOCK_REALTIME, &timeout);
            timeout.tv_sec += 10;  // 10 second timeout
            
            int result = pthread_cond_timedwait(&band->queue_condition, &band->queue_mutex, &timeout);
            if(result == ETIMEDOUT) {
                printf("Band %zu: Timeout waiting for new orders, continuing to check waiting queue\n", band->id);
                break; // Exit the while loop to continue processing
            }
        }


        if ( !KRUSTY_OPEN ){
            pthread_mutex_unlock(&band->queue_mutex);
            break;
        }
        
        // Check if we have an order after timeout
        if(queue_empty(&band->order_queue)) {
            pthread_mutex_unlock(&band->queue_mutex);
            continue; // Go back to check waiting queue again
        }
        
        order = dequeue_order(&band->order_queue);
        if( order == NULL) {
            pthread_mutex_unlock(&band->queue_mutex);
            continue; // Continue the loop instead of crashing
        }
        int result = cook_hamburger(order, band);
        //printf("ORDER %zu corresponds to the band ID : %zu\n", order->id, band->id);
        pthread_mutex_unlock(&band->queue_mutex);
        if( result != 1 ){
            printf(" BAND %zu FAILED TO COOK ORDER %zu NOT ENOUGH INGREDIENTS\n", band->id, order->id);
            pthread_mutex_lock(&band->waiting_queue_mutex);
            queue_order(&band->waiting_queue, order);
            pthread_mutex_unlock(&band->waiting_queue_mutex);
        }else{
            band_status(band);
            printf("ORDER %zu DELIVERED SUCESSFULLY by BAND %zu\n", order->id, band->id);
            printf("PREPARING TO TAKE ANOTHER REQUEST....\n");
        }
    }
}
void *show_band_states(void *args){
    bands_data_t* bands = (bands_data_t*) args;
    while(KRUSTY_OPEN){
        sleep(3);
        for(size_t i = 0; i<bands->size; i++){
            band_status_2(&bands->bands[i]);
        }
    }
}