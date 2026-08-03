#include "common.h"
#include "report.h"
#include "medicine.h"
#include "supplier.h"
#include "customer.h"
#include "sales.h"
#include "purchase.h"
#include "utils.h"

void salesReport(void) {
    int i;
    float total;
    total = 0;
    printf("\n--- Sales Report ---\n");
    for (i = 0; i < saleCount; i++) {
        printf("%-14s Qty:%-4d Total:%.2f\n", sales[i].medName, sales[i].qty, sales[i].total);
        total = total + sales[i].total;
    }
    printf("Grand Total Sales: %.2f\n", total);
}

void purchaseReport(void) {
    int i;
    float total;
    total = 0;
    printf("\n--- Purchase Report ---\n");
    for (i = 0; i < purchaseCount; i++) {
        printf("%-14s Qty:%-4d Total:%.2f\n", purchases[i].medName, purchases[i].qty, purchases[i].total);
        total = total + purchases[i].total;
    }
    printf("Grand Total Purchases: %.2f\n", total);
}

void profitReport(void) {
    int i, j;
    float cost, profit, totalProfit;
    totalProfit = 0;
    printf("\n--- Profit Report ---\n");
    for (i = 0; i < saleCount; i++) {
        cost = 0;
        for (j = 0; j < medicineCount; j++) {
            if (medicines[j].id == sales[i].medId) {
                cost = medicines[j].purchasePrice;
            }
        }
        profit = (sales[i].rate - cost) * (float)sales[i].qty;
        printf("%-14s Profit:%.2f\n", sales[i].medName, profit);
        totalProfit = totalProfit + profit;
    }
    printf("Total Profit: %.2f\n", totalProfit);
}

void reportMenu(void) {
    int choice;
    do {
        printf("\n--- Reports Menu ---\n");
        printf("1.Inventory 2.Sales 3.Purchases 4.Profit\n");
        printf("5.Suppliers 6.Customers 7.Expiry 8.Low Stock  0.Back\n");
        printf("Enter choice: "); choice = readInt();

        switch (choice) {
            case 1: viewMedicines(); break;
            case 2: salesReport(); break;
            case 3: purchaseReport(); break;
            case 4: profitReport(); break;
            case 5: viewSuppliers(); break;
            case 6: viewCustomers(); break;
            case 7: checkExpiry(); break;
            case 8: lowStockAlert(); break;
            default: break;
        }
    } while (choice != 0);
}
