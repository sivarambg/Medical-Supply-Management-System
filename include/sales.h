#ifndef SALES_H
#define SALES_H

#include "common.h"

struct Sale {
    int medId;
    char medName[NAME_LEN];
    char custName[NAME_LEN];
    int qty;
    float rate;
    float total;
};

extern struct Sale sales[MAX_SALE];
extern int saleCount;

/* Adds one sale record to the in-memory array (used by billing.c).
   Silently does nothing if the array is already full. */
void recordSale(int medId, const char *medName, const char *custName, int qty, float rate);

#endif /* SALES_H */
