#include "common.h"
#include "sales.h"

struct Sale sales[MAX_SALE];
int saleCount = 0;

void recordSale(int medId, const char *medName, const char *custName, int qty, float rate) {
    if (saleCount < MAX_SALE) {
        sales[saleCount].medId = medId;
        strcpy(sales[saleCount].medName, medName);
        strcpy(sales[saleCount].custName, custName);
        sales[saleCount].qty = qty;
        sales[saleCount].rate = rate;
        sales[saleCount].total = (float)qty * rate;
        saleCount++;
    }
}
