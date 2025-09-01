#ifndef DISPENSER_H
#define DISPENSER_H

#include "../struct.h"

band_t init_band();
int cook_hamburger(order_t *order, band_t *band);
int band_status(band_t *band);
band_t *initialize_bands(size_t n);
void clean_bands(band_t *bands, int n);



#endif