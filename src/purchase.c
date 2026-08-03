#include "common.h"
#include "purchase.h"

struct Purchase purchases[MAX_PUR];
int purchaseCount = 0;

void recordPurchase(int medId, const char *medName, const char *suppName, int qty, float rate) {
    if (purchaseCount < MAX_PUR) {
        purchases[purchaseCount].medId = medId;
        strcpy(purchases[purchaseCount].medName, medName);
        strcpy(purchases[purchaseCount].suppName, suppName);
        purchases[purchaseCount].qty = qty;
        purchases[purchaseCount].rate = rate;
        purchases[purchaseCount].total = (float)qty * rate;
        purchaseCount++;
    }
}
