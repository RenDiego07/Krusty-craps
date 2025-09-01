
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#include "./dispenser/dispenser.h"
#include "./order/order.h"
#include "./queue/queue.h"
#include "./struct.h"



int N;
bool KRUSTY_OPEN = true;
char *file;
bool ALL_ORDERS_TAKEN = false;
int NUM_ORDER = 0;

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
    pthread_t tid [N];

    for(int i = 0; i < N; i++){
        if(pthread_create(&tid[i], NULL, band_handler, &bands[i]) != 0){
            printf("COULD NOT INITIALIZE THREADS FOR BAND %d\n", i);
            exit(1);
        }
        //band_status(&bands[i]);
        printf("\n");
    }
    char request[10];
    int band_id = 0;
    size_t times = 0;

    while(fgets(request, sizeof(request), fp) || ALL_ORDERS_TAKEN){
        
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
    KRUSTY_OPEN = false;
    for(int i = 0; i < N; i++) {
      pthread_join(tid[i], NULL);  
    }
    clean_bands(bands, N);


    return 0;
}

void* band_handler( void* arg){
    band_t *band = (band_t *) arg;
    while( KRUSTY_OPEN ){
        order_t *order = NULL;
        pthread_mutex_lock(&band->queue_mutex);
        while(queue_empty(&band->order_queue) && KRUSTY_OPEN){
            pthread_cond_wait(&band->queue_condition, &band->queue_mutex);
        }


        if ( !KRUSTY_OPEN ){
            pthread_mutex_unlock(&band->queue_mutex);
            break;
        }
        order = dequeue_order(&band->order_queue);
        if( order == NULL) {
            printf("ORDER IS NULL\n");
            exit(1);
        }
        int result = cook_hamburger(order, band);
        //printf("ORDER %zu corresponds to the band ID : %zu\n", order->id, band->id);
        pthread_mutex_unlock(&band->queue_mutex);
        if( result != 0 ){
            printf(" BAND %zu FAILED TO COOK ORDER %zu NOT ENOUGH INGREDIENTS\n", band->id, order->id);
            pthread_mutex_lock(&band->waiting_queue_mutex);
            queue_order(&band->waiting_queue, order);
            pthread_mutex_unlock(&band->waiting_queue_mutex);
        }else{
            band_status(band);
            printf("ORDER %zu DELIVERED SUCESSFULLY by BAND %zu\n", order->id, band->id);
            printf("PREPARING TO TAKE ANOTHER REQUEST....\n");
        }
    // aqui ya existe una orden. Tengo que desencolar
    }

}