#ifndef MEDICINE_H
#define MEDICINE_H

#include "common.h"

struct Medicine {
    int id;
    char name[NAME_LEN];
    char batch[SHORT_LEN];
    char company[NAME_LEN];
    char supplier[NAME_LEN];
    float purchasePrice;
    float sellingPrice;
    int quantity;
    int minStock;
    int expDay, expMonth, expYear;   /* stored as 3 ints - easy to compare, no string parsing needed */
};

/* Single in-memory "table" of medicines, defined in medicine.c,
   shared with billing.c and report.c through this extern. */
extern struct Medicine medicines[MAX_MED];
extern int medicineCount;

extern const char *medicineFileName;

/* ---- File storage ---- */
void saveMedicine(struct Medicine m);   /* append one new record */
void saveAllMedicines(void);            /* rewrite the whole file from the array */
void loadMedicines(void);               /* read the whole file back into the array */

/* ---- CRUD / stock operations ---- */
void addMedicine(void);
void viewMedicines(void);
void searchMedicine(void);
void editMedicine(void);
void deleteMedicine(void);
void purchaseMedicine(void);   /* restock: increases quantity */

/* ---- Alerts ---- */
void checkExpiry(void);
void lowStockAlert(void);

/* Returns 1 if medicines[index]'s expiry date is strictly before the
   given date, 0 otherwise. Shared by checkExpiry() here and the
   startup alerts screen in menu.c, so the comparison logic only
   exists in one place. */
int medicineIsExpired(int index, int day, int month, int year);

/* ---- Menu ---- */
void medicineMenu(void);

#endif /* MEDICINE_H */
