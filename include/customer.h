#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "common.h"

struct Customer {
    int id;
    char name[NAME_LEN];
    char city[NAME_LEN];
    char mobile[MOBILE_LEN];
};

extern struct Customer customers[MAX_CUST];
extern int customerCount;

extern const char *customerFileName;

/* ---- File storage ---- */
void saveCustomer(struct Customer c);
void saveAllCustomers(void);
void loadCustomers(void);

/* ---- CRUD ---- */
void addCustomer(void);
void viewCustomers(void);
void searchCustomer(void);
void editCustomer(void);
void deleteCustomer(void);
void purchaseHistory(void);

/* Looks a customer up by mobile number. Returns the array index if
   found, or -1 if not found. Used by billing.c to gate billing on
   the customer already existing. */
int findCustomerByMobile(const char *mobile);

/* ---- Menu ---- */
void customerMenu(void);

#endif /* CUSTOMER_H */
