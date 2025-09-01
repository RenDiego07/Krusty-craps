#ifndef ORDER_H
#define ORDER_H

#include "../struct.h"
#include <stddef.h>

order_t *init_order(char *stream, size_t length);

order_data_t *get_info(order_t *order);
#endif