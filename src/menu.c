#include "common.h"
#include "menu.h"
#include "medicine.h"
#include "supplier.h"
#include "customer.h"
#include "billing.h"
#include "report.h"
#include "about.h"
#include "user.h"
#include "utils.h"

void startupAlerts(void) {
    int i, day, month, year, expiredCount, lowStockCount;
    expiredCount = 0;
    lowStockCount = 0;
    getCurrentDate(&day, &month, &year);

    for (i = 0; i < medicineCount; i++) {
        if (medicineIsExpired(i, day, month, year)) {
            expiredCount++;
        }
        if (medicines[i].quantity <= medicines[i].minStock) {
            lowStockCount++;
        }
    }

    printf("\n===== SYSTEM ALERTS =====\n");
    printf("Expired Medicines   : %d\n", expiredCount);
    printf("Low Stock Medicines : %d\n", lowStockCount);
    printf("==========================\n");
}

void mainMenu(void) {
    int choice;
    do {
        printf("\n===== MAIN MENU =====\n");
        printf("1.Medicine Management\n");
        printf("2.Supplier Management\n");
        printf("3.Customer Management\n");
        printf("4.Generate Bill\n");
        printf("5.Reports\n");
        printf("6.About\n");
        if (currentUserIsAdmin) {
            printf("7.Approve Pending Users (Admin)\n");
        }
        printf("0.Exit\n");
        printf("Enter choice: "); choice = readInt();

        switch (choice) {
            case 1: medicineMenu(); break;
            case 2: supplierMenu(); break;
            case 3: customerMenu(); break;
            case 4: generateBill(); break;
            case 5: reportMenu(); break;
            case 6: showAbout(); break;
            case 7: approvePendingUsers(); break;
            default: break;
        }
    } while (choice != 0);

    printf("Thank you for using MediStore. Goodbye!\n");
}
