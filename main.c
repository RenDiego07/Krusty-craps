#include <stdlib.h>
#include <stdio.h>
#include "./struct.h"
#include "./dispenser/dispenser.h"
#include "./order/order.h"
#include <string.h>


int N;

int main (int argc, char *argv[]){
    if ( argc < 2 ){
        printf("DID NOT PASS ENOUGH ARGUMENTS\n");
        exit(1);
    }
    N = atoi(argv[1]);
    band_t *bands = initialize_bands(N);
    for(int i = 0; i < N; i++){
        printf("BAND %d\n", i);
        band_status(&bands[i]);
        printf("\n");
    }
    char *bigMc = "BMMMLTB";
    order_t *order = init_order(bigMc, strlen(bigMc));

    for( int index = 0; index<N; index++){
        int result = cook_hamburger(order, &bands[index]);
        if( result == 0){
            printf("BAND %d DELIVERED A HAMBURGER\n", index);
            
        }else{
            printf("BAND %d DOESN'T HAVE ENOUGH INGREDIENTS TO MAKE THE HAMBURGER\n", index);
        }
    }

    // band_t *band = init_band();
    // band_status(band);
    // printf("BEFORE\n");
    // char *receipt = "BMLMCCTTB";
    // size_t len = strlen(receipt);
    // order_t *order = init_order(receipt,len);

    // printf("AFTER\n");
    
    // int result = cook_hamburger(order, band);
    // band_status(band);

    
    return 0;
}