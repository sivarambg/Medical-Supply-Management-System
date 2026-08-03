#include "common.h"
#include "medicine.h"
#include "utils.h"

/* NOTE on purchases: the actual restock TRANSACTION records (who
   supplied how much, at what rate) live in purchase.h/purchase.c,
   since that data is really "purchase history", not the medicine's
   own master record. purchaseMedicine() below calls into it. */
#include "purchase.h"

struct Medicine medicines[MAX_MED];
int medicineCount = 0;

const char *medicineFileName = "medicine.txt";

/* ============================================================
   FILE STORAGE
   ============================================================ */

void saveMedicine(struct Medicine m) {
    FILE *fp;
    fp = fopen(medicineFileName, "a");   /* append = add to the end, keep old data */
    if (fp != NULL) {
        fprintf(fp, "%d,%s,%s,%s,%s,%.2f,%.2f,%d,%d,%d,%d,%d\n",
            m.id, m.name, m.batch, m.company, m.supplier,
            m.purchasePrice, m.sellingPrice, m.quantity, m.minStock,
            m.expDay, m.expMonth, m.expYear);
        fclose(fp);
    }
}

void saveAllMedicines(void) {
    FILE *fp;
    int i;
    fp = fopen(medicineFileName, "w");   /* overwrite with the current, up-to-date list */
    if (fp == NULL) {
        return;
    }
    for (i = 0; i < medicineCount; i++) {
        fprintf(fp, "%d,%s,%s,%s,%s,%.2f,%.2f,%d,%d,%d,%d,%d\n",
            medicines[i].id, medicines[i].name, medicines[i].batch, medicines[i].company, medicines[i].supplier,
            medicines[i].purchasePrice, medicines[i].sellingPrice, medicines[i].quantity, medicines[i].minStock,
            medicines[i].expDay, medicines[i].expMonth, medicines[i].expYear);
    }
    fclose(fp);
}

void loadMedicines(void) {
    FILE *fp;
    struct Medicine m;
    medicineCount = 0;
    fp = fopen(medicineFileName, "r");   /* read mode */
    if (fp == NULL) {
        return;   /* file does not exist yet - start with an empty list, that is fine */
    }
    while (fscanf(fp, "%d,%29[^,],%19[^,],%29[^,],%29[^,],%f,%f,%d,%d,%d,%d,%d\n",
            &m.id, m.name, m.batch, m.company, m.supplier,
            &m.purchasePrice, &m.sellingPrice, &m.quantity, &m.minStock,
            &m.expDay, &m.expMonth, &m.expYear) == 12) {
        if (medicineCount < MAX_MED) {
            medicines[medicineCount] = m;
            medicineCount++;
        }
    }
    fclose(fp);
}

/* ============================================================
   CRUD / STOCK OPERATIONS
   ============================================================ */

void addMedicine(void) {
    struct Medicine m;
    char line[LOG_LINE_LEN];

    if (medicineCount >= MAX_MED) {
        printf("Medicine list is full!\n");
        return;
    }

    m.id = medicineCount + 1;   /* simple auto-increment ID */

    printf("Enter medicine name   : "); scanf(" %29[^\n]", m.name);
    printf("Enter batch number    : "); scanf(" %19[^\n]", m.batch);
    printf("Enter company         : "); scanf(" %29[^\n]", m.company);
    printf("Enter supplier name   : "); scanf(" %29[^\n]", m.supplier);
    printf("Enter purchase price  : "); scanf("%f", &m.purchasePrice);
    printf("Enter selling price   : "); scanf("%f", &m.sellingPrice);
    printf("Enter quantity        : "); m.quantity = readInt();
    printf("Enter minimum stock   : "); m.minStock = readInt();
    printf("Enter expiry (dd mm yyyy): "); scanf("%d %d %d", &m.expDay, &m.expMonth, &m.expYear);

    medicines[medicineCount] = m;
    medicineCount++;

    saveMedicine(m);   /* append this new record to medicine.txt */

    sprintf(line, "ADD MEDICINE: ID=%d Name=%s Qty=%d", m.id, m.name, m.quantity);
    writeLog(line);

    printf("Medicine added successfully!\n");
}

void viewMedicines(void) {
    int i;
    printf("\nID   Name           Qty   MinStock  Expiry\n");
    for (i = 0; i < medicineCount; i++) {
        printf("%-4d %-14s %-5d %-9d %02d-%02d-%04d\n",
            medicines[i].id, medicines[i].name, medicines[i].quantity,
            medicines[i].minStock, medicines[i].expDay, medicines[i].expMonth, medicines[i].expYear);
    }
    if (medicineCount == 0) {
        printf("No medicines found.\n");
    }
}

void searchMedicine(void) {
    char key[NAME_LEN];
    int i, id, found;
    found = 0;

    printf("Enter medicine name or ID to search: ");
    scanf(" %29[^\n]", key);
    id = atoi(key);   /* if the input was a number, this becomes the ID; 0 otherwise */

    for (i = 0; i < medicineCount; i++) {
        if ((medicines[i].id == id) || (strstr(medicines[i].name, key) != NULL)) {
            printf("ID:%d Name:%s Qty:%d Price:%.2f Expiry:%02d-%02d-%04d\n",
                medicines[i].id, medicines[i].name, medicines[i].quantity,
                medicines[i].sellingPrice, medicines[i].expDay, medicines[i].expMonth, medicines[i].expYear);
            found = 1;
        }
    }
    if (!found) {
        printf("Medicine not found.\n");
    }
}

void editMedicine(void) {
    int id, i;
    printf("Enter Medicine ID to edit: "); id = readInt();

    for (i = 0; i < medicineCount; i++) {
        if (medicines[i].id == id) {
            printf("Enter new quantity      : "); medicines[i].quantity = readInt();
            printf("Enter new selling price : "); scanf("%f", &medicines[i].sellingPrice);
            saveAllMedicines();   /* rewrite medicine.txt with the updated data */
            writeLog("EDIT MEDICINE performed");
            printf("Medicine updated!\n");
            return;
        }
    }
    printf("Medicine ID not found.\n");
}

void deleteMedicine(void) {
    int id, i, j;
    printf("Enter Medicine ID to delete: "); id = readInt();

    for (i = 0; i < medicineCount; i++) {
        if (medicines[i].id == id) {
            /* shift every later record one place left - simple array delete, no pointers */
            for (j = i; j < (medicineCount - 1); j++) {
                medicines[j] = medicines[j + 1];
            }
            medicineCount--;
            saveAllMedicines();   /* rewrite medicine.txt without the deleted record */
            writeLog("DELETE MEDICINE performed");
            printf("Medicine deleted!\n");
            return;
        }
    }
    printf("Medicine ID not found.\n");
}

/* NOTE: single-item "sell" was replaced by billing.c's generateBill(),
   which handles one or more medicines in a single customer visit,
   shows a proper printed-style bill, and reduces stock for every item. */

void purchaseMedicine(void) {
    int id, qty, i;

    printf("Enter Medicine ID: "); id = readInt();

    for (i = 0; i < medicineCount; i++) {
        if (medicines[i].id == id) {
            printf("Enter quantity purchased: "); qty = readInt();
            medicines[i].quantity = medicines[i].quantity + qty;

            recordPurchase(id, medicines[i].name, medicines[i].supplier, qty, medicines[i].purchasePrice);

            saveAllMedicines();   /* rewrite medicine.txt with the increased stock */

            printf("Stock updated. New quantity = %d\n", medicines[i].quantity);
            return;
        }
    }
    printf("Medicine ID not found.\n");
}

/* ============================================================
   ALERTS
   ============================================================ */

int medicineIsExpired(int index, int day, int month, int year) {
    int expired;
    expired = 0;
    if (medicines[index].expYear < year) {
        expired = 1;
    } else if ((medicines[index].expYear == year) && (medicines[index].expMonth < month)) {
        expired = 1;
    } else if ((medicines[index].expYear == year) && (medicines[index].expMonth == month) && (medicines[index].expDay < day)) {
        expired = 1;
    } else {
        expired = 0;
    }
    return expired;
}

/* Expired + expiring-soon medicines, compared against today's date. */
void checkExpiry(void) {
    int day, month, year, i;
    int totalToday, totalExpiry, diff;

    getCurrentDate(&day, &month, &year);

    printf("\n--- Expired Medicines ---\n");
    for (i = 0; i < medicineCount; i++) {
        if (medicineIsExpired(i, day, month, year)) {
            printf("%s (Expired on %02d-%02d-%04d)\n", medicines[i].name,
                medicines[i].expDay, medicines[i].expMonth, medicines[i].expYear);
        }
    }

    /* Approximate day-count using 30 days/month, 360 days/year - simple,
       good enough for a "within 30 days" warning without needing full
       calendar/leap-year math. */
    totalToday = (year * 360) + (month * 30) + day;

    printf("\n--- Expiring Within 30 Days ---\n");
    for (i = 0; i < medicineCount; i++) {
        totalExpiry = (medicines[i].expYear * 360) + (medicines[i].expMonth * 30) + medicines[i].expDay;
        diff = totalExpiry - totalToday;
        if ((diff >= 0) && (diff <= 30)) {
            printf("%s (Expires on %02d-%02d-%04d)\n", medicines[i].name,
                medicines[i].expDay, medicines[i].expMonth, medicines[i].expYear);
        }
    }
}

void lowStockAlert(void) {
    int i, found;
    found = 0;
    printf("\n--- Low Stock Medicines ---\n");
    for (i = 0; i < medicineCount; i++) {
        if (medicines[i].quantity <= medicines[i].minStock) {
            printf("%s: Current=%d Minimum=%d\n", medicines[i].name, medicines[i].quantity, medicines[i].minStock);
            found = 1;
        }
    }
    if (!found) {
        printf("No low stock medicines.\n");
    }
}

/* ============================================================
   MENU
   ============================================================ */

void medicineMenu(void) {
    int choice;
    do {
        printf("\n--- Medicine Menu ---\n");
        printf("1.Add  2.Edit  3.Delete  4.Search  5.List\n");
        printf("6.Purchase(Restock)  7.Low Stock Alerts  0.Back\n");
        printf("Enter choice: "); choice = readInt();

        switch (choice) {
            case 1: addMedicine(); break;
            case 2: editMedicine(); break;
            case 3: deleteMedicine(); break;
            case 4: searchMedicine(); break;
            case 5: viewMedicines(); break;
            case 6: purchaseMedicine(); break;
            case 7: lowStockAlert(); break;
            default: break;
        }
    } while (choice != 0);
}
