#include "common.h"
#include "customer.h"
#include "utils.h"
#include "sales.h"

struct Customer customers[MAX_CUST];
int customerCount = 0;

const char *customerFileName = "customer.txt";

/* ============================================================
   FILE STORAGE
   ============================================================ */

void saveCustomer(struct Customer c) {
    FILE *fp;
    fp = fopen(customerFileName, "a");
    if (fp != NULL) {
        fprintf(fp, "%d,%s,%s,%s\n", c.id, c.name, c.city, c.mobile);
        fclose(fp);
    }
}

void saveAllCustomers(void) {
    FILE *fp;
    int i;
    fp = fopen(customerFileName, "w");
    if (fp == NULL) {
        return;
    }
    for (i = 0; i < customerCount; i++) {
        fprintf(fp, "%d,%s,%s,%s\n", customers[i].id, customers[i].name, customers[i].city, customers[i].mobile);
    }
    fclose(fp);
}

void loadCustomers(void) {
    FILE *fp;
    struct Customer c;
    customerCount = 0;
    fp = fopen(customerFileName, "r");
    if (fp == NULL) {
        return;
    }
    while (fscanf(fp, "%d,%29[^,],%29[^,],%14[^\n]\n", &c.id, c.name, c.city, c.mobile) == 4) {
        if (customerCount < MAX_CUST) {
            customers[customerCount] = c;
            customerCount++;
        }
    }
    fclose(fp);
}

/* ============================================================
   CRUD
   ============================================================ */

void addCustomer(void) {
    struct Customer c;
    if (customerCount >= MAX_CUST) {
        printf("Customer list is full!\n");
        return;
    }
    c.id = customerCount + 1;
    printf("Enter customer name : "); scanf(" %29[^\n]", c.name);
    printf("Enter city          : "); scanf(" %29[^\n]", c.city);
    printf("Enter mobile number : "); scanf(" %14[^\n]", c.mobile);

    customers[customerCount] = c;
    customerCount++;

    saveCustomer(c);   /* append this new record to customer.txt */

    writeLog("ADD CUSTOMER performed");
    printf("Customer added successfully!\n");
}

void viewCustomers(void) {
    int i;
    printf("\nID   Name           City           Mobile\n");
    for (i = 0; i < customerCount; i++) {
        printf("%-4d %-14s %-14s %-14s\n", customers[i].id, customers[i].name, customers[i].city, customers[i].mobile);
    }
    if (customerCount == 0) {
        printf("No customers found.\n");
    }
}

void searchCustomer(void) {
    char key[NAME_LEN];
    int i, found;
    found = 0;
    printf("Enter customer name or mobile number to search: "); scanf(" %29[^\n]", key);

    for (i = 0; i < customerCount; i++) {
        if ((strstr(customers[i].name, key) != NULL) || (strstr(customers[i].mobile, key) != NULL)) {
            printf("ID:%d Name:%s City:%s Mobile:%s\n", customers[i].id, customers[i].name, customers[i].city, customers[i].mobile);
            found = 1;
        }
    }
    if (!found) {
        printf("Customer not found.\n");
    }
}

void editCustomer(void) {
    int id, i;
    printf("Enter Customer ID to edit: "); id = readInt();
    for (i = 0; i < customerCount; i++) {
        if (customers[i].id == id) {
            printf("Enter new mobile number: "); scanf(" %14[^\n]", customers[i].mobile);
            saveAllCustomers();
            printf("Customer updated!\n");
            return;
        }
    }
    printf("Customer ID not found.\n");
}

void deleteCustomer(void) {
    int id, i, j;
    printf("Enter Customer ID to delete: "); id = readInt();
    for (i = 0; i < customerCount; i++) {
        if (customers[i].id == id) {
            for (j = i; j < (customerCount - 1); j++) {
                customers[j] = customers[j + 1];
            }
            customerCount--;
            saveAllCustomers();
            printf("Customer deleted!\n");
            return;
        }
    }
    printf("Customer ID not found.\n");
}

void purchaseHistory(void) {
    char name[NAME_LEN];
    int i, found;
    float total;
    found = 0;
    total = 0;

    printf("Enter customer name: "); scanf(" %29[^\n]", name);
    printf("\nMedicine        Qty   Rate    Total\n");

    for (i = 0; i < saleCount; i++) {
        if (strstr(sales[i].custName, name) != NULL) {
            printf("%-14s %-5d %-7.2f %-7.2f\n", sales[i].medName, sales[i].qty, sales[i].rate, sales[i].total);
            total = total + sales[i].total;
            found = 1;
        }
    }
    if (!found) {
        printf("No purchase history found.\n");
    } else {
        printf("Grand Total: %.2f\n", total);
    }
}

int findCustomerByMobile(const char *mobile) {
    int i;
    for (i = 0; i < customerCount; i++) {
        if (strcmp(customers[i].mobile, mobile) == 0) {
            return i;
        }
    }
    return -1;
}

/* ============================================================
   MENU
   ============================================================ */

void customerMenu(void) {
    int choice;
    do {
        printf("\n--- Customer Menu ---\n");
        printf("1.Add  2.Edit  3.Delete  4.Search  5.List  6.Purchase History  0.Back\n");
        printf("Enter choice: "); choice = readInt();

        switch (choice) {
            case 1: addCustomer(); break;
            case 2: editCustomer(); break;
            case 3: deleteCustomer(); break;
            case 4: searchCustomer(); break;
            case 5: viewCustomers(); break;
            case 6: purchaseHistory(); break;
            default: break;
        }
    } while (choice != 0);
}
