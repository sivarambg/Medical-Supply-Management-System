#include "common.h"
#include "supplier.h"
#include "utils.h"

struct Supplier suppliers[MAX_SUP];
int supplierCount = 0;

const char *supplierFileName = "supplier.txt";

/* ============================================================
   FILE STORAGE
   ============================================================ */

void saveSupplier(struct Supplier s) {
    FILE *fp;
    fp = fopen(supplierFileName, "a");
    if (fp != NULL) {
        fprintf(fp, "%d,%s,%s,%s\n", s.id, s.name, s.city, s.mobile);
        fclose(fp);
    }
}

void saveAllSuppliers(void) {
    FILE *fp;
    int i;
    fp = fopen(supplierFileName, "w");
    if (fp == NULL) {
        return;
    }
    for (i = 0; i < supplierCount; i++) {
        fprintf(fp, "%d,%s,%s,%s\n", suppliers[i].id, suppliers[i].name, suppliers[i].city, suppliers[i].mobile);
    }
    fclose(fp);
}

void loadSuppliers(void) {
    FILE *fp;
    struct Supplier s;
    supplierCount = 0;
    fp = fopen(supplierFileName, "r");
    if (fp == NULL) {
        return;
    }
    while (fscanf(fp, "%d,%29[^,],%29[^,],%14[^\n]\n", &s.id, s.name, s.city, s.mobile) == 4) {
        if (supplierCount < MAX_SUP) {
            suppliers[supplierCount] = s;
            supplierCount++;
        }
    }
    fclose(fp);
}

/* ============================================================
   CRUD
   ============================================================ */

void addSupplier(void) {
    struct Supplier s;
    if (supplierCount >= MAX_SUP) {
        printf("Supplier list is full!\n");
        return;
    }
    s.id = supplierCount + 1;
    printf("Enter supplier name : "); scanf(" %29[^\n]", s.name);
    printf("Enter city          : "); scanf(" %29[^\n]", s.city);
    printf("Enter mobile number : "); scanf(" %14[^\n]", s.mobile);

    suppliers[supplierCount] = s;
    supplierCount++;

    saveSupplier(s);   /* append this new record to supplier.txt */

    writeLog("ADD SUPPLIER performed");
    printf("Supplier added successfully!\n");
}

void viewSuppliers(void) {
    int i;
    printf("\nID   Name           City           Mobile\n");
    for (i = 0; i < supplierCount; i++) {
        printf("%-4d %-14s %-14s %-14s\n", suppliers[i].id, suppliers[i].name, suppliers[i].city, suppliers[i].mobile);
    }
    if (supplierCount == 0) {
        printf("No suppliers found.\n");
    }
}

void searchSupplier(void) {
    char key[NAME_LEN];
    int i, found;
    found = 0;
    printf("Enter supplier name or mobile number to search: "); scanf(" %29[^\n]", key);

    for (i = 0; i < supplierCount; i++) {
        if ((strstr(suppliers[i].name, key) != NULL) || (strstr(suppliers[i].mobile, key) != NULL)) {
            printf("ID:%d Name:%s City:%s Mobile:%s\n", suppliers[i].id, suppliers[i].name, suppliers[i].city, suppliers[i].mobile);
            found = 1;
        }
    }
    if (!found) {
        printf("Supplier not found.\n");
    }
}

void editSupplier(void) {
    int id, i;
    printf("Enter Supplier ID to edit: "); id = readInt();
    for (i = 0; i < supplierCount; i++) {
        if (suppliers[i].id == id) {
            printf("Enter new mobile number: "); scanf(" %14[^\n]", suppliers[i].mobile);
            saveAllSuppliers();
            printf("Supplier updated!\n");
            return;
        }
    }
    printf("Supplier ID not found.\n");
}

void deleteSupplier(void) {
    int id, i, j;
    printf("Enter Supplier ID to delete: "); id = readInt();
    for (i = 0; i < supplierCount; i++) {
        if (suppliers[i].id == id) {
            for (j = i; j < (supplierCount - 1); j++) {
                suppliers[j] = suppliers[j + 1];
            }
            supplierCount--;
            saveAllSuppliers();
            printf("Supplier deleted!\n");
            return;
        }
    }
    printf("Supplier ID not found.\n");
}

/* ============================================================
   MENU
   ============================================================ */

void supplierMenu(void) {
    int choice;
    do {
        printf("\n--- Supplier Menu ---\n");
        printf("1.Add  2.Edit  3.Delete  4.Search  5.List  0.Back\n");
        printf("Enter choice: "); choice = readInt();

        switch (choice) {
            case 1: addSupplier(); break;
            case 2: editSupplier(); break;
            case 3: deleteSupplier(); break;
            case 4: searchSupplier(); break;
            case 5: viewSuppliers(); break;
            default: break;
        }
    } while (choice != 0);
}
