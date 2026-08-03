#ifndef PURCHASE_H
#define PURCHASE_H

#include "common.h"

struct Purchase {
    int medId;
    char medName[NAME_LEN];
    char suppName[NAME_LEN];
    int qty;
    float rate;
    float total;
};

extern struct Purchase purchases[MAX_PUR];
extern int purchaseCount;

/* Adds one restock/purchase record to the in-memory array
   (used by medicine.c's purchaseMedicine()). Silently does
   nothing if the array is already full. */
void recordPurchase(int medId, const char *medName, const char *suppName, int qty, float rate);

#endif /* PURCHASE_H */
