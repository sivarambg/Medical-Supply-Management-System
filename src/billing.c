#include "common.h"
#include "billing.h"
#include "medicine.h"
#include "customer.h"
#include "sales.h"
#include "utils.h"

const char *billFileName = "bills.txt";

void generateBill(void) {
    char custName[NAME_LEN];
    char custMobile[MOBILE_LEN];
    int itemId[MAX_BILL_ITEMS] = {0};
    char itemName[MAX_BILL_ITEMS][NAME_LEN] = {{0}};
    int itemQty[MAX_BILL_ITEMS] = {0};
    float itemRate[MAX_BILL_ITEMS] = {0.0f};
    float itemTotal[MAX_BILL_ITEMS] = {0.0f};
    int itemCount;
    int id, qty, i, medIndex, custIndex;
    float grandTotal;
    char line[LOG_LINE_LEN];
    char addNow;
    struct Customer newCust;
    FILE *fp;

    itemCount = 0;
    grandTotal = 0;

    /* ---- billing is only valid for a customer already in customer.txt ---- */
    printf("Enter customer mobile number: ");
    scanf(" %14[^\n]", custMobile);

    custIndex = findCustomerByMobile(custMobile);

    if (custIndex == -1) {
        printf("No customer found with mobile number %s.\n", custMobile);
        printf("Add this customer now before billing? (y/n): ");
        scanf(" %c", &addNow);

        if ((addNow != 'y') && (addNow != 'Y')) {
            printf("Billing cancelled. Please add the customer first.\n");
            return;
        }

        if (customerCount >= MAX_CUST) {
            printf("Customer list is full! Cannot add.\n");
            return;
        }

        newCust.id = customerCount + 1;
        strcpy(newCust.mobile, custMobile);
        printf("Enter customer name: "); scanf(" %29[^\n]", newCust.name);
        printf("Enter city          : "); scanf(" %29[^\n]", newCust.city);

        customers[customerCount] = newCust;
        customerCount++;
        saveCustomer(newCust);   /* append this new record to customer.txt */
        writeLog("ADD CUSTOMER performed (via billing)");

        custIndex = customerCount - 1;
        printf("Customer added.\n");
    }

    strcpy(custName, customers[custIndex].name);
    printf("Billing for: %s (Mobile: %s)\n", custName, customers[custIndex].mobile);

    printf("Now add medicines to the bill.\n");
    printf("Enter Medicine ID as 0 when you are done.\n");

    while (itemCount < MAX_BILL_ITEMS) {
        printf("\nMedicine ID (0 to finish): ");
        id = readInt();
        if (id == 0) {
            break;
        }

        /* find the medicine by ID */
        medIndex = -1;
        for (i = 0; i < medicineCount; i++) {
            if (medicines[i].id == id) {
                medIndex = i;
            }
        }
        if (medIndex == -1) {
            printf("Medicine ID not found. Try again.\n");
            continue;
        }

        printf("Medicine: %s   Available Qty: %d\n", medicines[medIndex].name, medicines[medIndex].quantity);
        printf("Enter quantity: ");
        qty = readInt();

        if ((qty <= 0) || (qty > medicines[medIndex].quantity)) {
            printf("Invalid quantity or not enough stock!\n");
            continue;
        }

        /* reduce stock right away */
        medicines[medIndex].quantity = medicines[medIndex].quantity - qty;

        /* remember this item for the printed bill */
        itemId[itemCount] = id;
        strcpy(itemName[itemCount], medicines[medIndex].name);
        itemQty[itemCount] = qty;
        itemRate[itemCount] = medicines[medIndex].sellingPrice;
        itemTotal[itemCount] = (float)qty * medicines[medIndex].sellingPrice;
        grandTotal = grandTotal + itemTotal[itemCount];

        /* also keep a sale record, same as the reports use elsewhere */
        recordSale(id, medicines[medIndex].name, custName, qty, medicines[medIndex].sellingPrice);

        itemCount++;
        printf("Added to bill.\n");
    }

    if (itemCount == 0) {
        printf("No items added. Bill cancelled.\n");
        return;
    }

    /* ---- show the bill on screen ---- */
    printf("\n================ BILL ================\n");
    printf("Customer: %s\n", custName);
    printf("---------------------------------------\n");
    printf("%-14s %5s %8s %8s\n", "Medicine", "Qty", "Rate", "Total");
    for (i = 0; i < itemCount; i++) {
        printf("%-14s %5d %8.2f %8.2f\n", itemName[i], itemQty[i], itemRate[i], itemTotal[i]);
    }
    printf("---------------------------------------\n");
    printf("Grand Total: %.2f\n", grandTotal);
    printf("========================================\n");

    /* ---- save the same bill into bills.txt (append mode) ---- */
    fp = fopen(billFileName, "a");
    if (fp != NULL) {
        fprintf(fp, "================ BILL ================\n");
        fprintf(fp, "Customer: %s\n", custName);
        fprintf(fp, "---------------------------------------\n");
        for (i = 0; i < itemCount; i++) {
            fprintf(fp, "%-14s %5d %8.2f %8.2f\n", itemName[i], itemQty[i], itemRate[i], itemTotal[i]);
        }
        fprintf(fp, "---------------------------------------\n");
        fprintf(fp, "Grand Total: %.2f\n", grandTotal);
        fprintf(fp, "========================================\n\n");
        fclose(fp);
    }

    /* ---- save the reduced stock levels back to medicine.txt ---- */
    saveAllMedicines();

    sprintf(line, "BILL: Customer=%s Items=%d GrandTotal=%.2f", custName, itemCount, grandTotal);
    writeLog(line);

    /* warn about any item that is now low on stock */
    for (i = 0; i < itemCount; i++) {
        for (medIndex = 0; medIndex < medicineCount; medIndex++) {
            if ((medicines[medIndex].id == itemId[i]) && (medicines[medIndex].quantity <= medicines[medIndex].minStock)) {
                printf("LOW STOCK WARNING: %s now at %d (minimum %d)\n",
                    medicines[medIndex].name, medicines[medIndex].quantity, medicines[medIndex].minStock);
            }
        }
    }
}
