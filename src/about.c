#include "common.h"
#include "about.h"

static const char *programVersion UNUSED = "1.0";

void showAbout(void) {
    printf("\nMediStore - Medicine & Pharmacy Management System\n");
    printf("Version %s - Beginner-Friendly C Edition\n", programVersion);
    printf("Features: Medicine, Supplier, Customer management, Reports, Expiry & Stock Alerts\n");
}
