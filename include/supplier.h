#ifndef SUPPLIER_H
#define SUPPLIER_H

#include "common.h"

struct Supplier {
    int id;
    char name[NAME_LEN];
    char city[NAME_LEN];
    char mobile[MOBILE_LEN];
};

extern struct Supplier suppliers[MAX_SUP];
extern int supplierCount;

extern const char *supplierFileName;

/* ---- File storage ---- */
void saveSupplier(struct Supplier s);
void saveAllSuppliers(void);
void loadSuppliers(void);

/* ---- CRUD ---- */
void addSupplier(void);
void viewSuppliers(void);
void searchSupplier(void);
void editSupplier(void);
void deleteSupplier(void);

/* ---- Menu ---- */
void supplierMenu(void);

#endif /* SUPPLIER_H */
