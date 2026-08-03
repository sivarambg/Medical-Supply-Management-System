#include "common.h"
#include "menu.h"
#include "medicine.h"
#include "supplier.h"
#include "customer.h"
#include "user.h"
#include "utils.h"

int main(void) {
    int choice;

    printf("Welcome to MediStore!\n");

    /* load everything saved from previous runs */
    loadUsers();
    loadMedicines();
    loadSuppliers();
    loadCustomers();

    /* keep showing Login/Register until someone logs in, or chooses Exit */
    while (1) {
        printf("\n===== WELCOME =====\n");
        printf("1.Login\n");
        printf("2.Register as New User\n");
        printf("0.Exit\n");
        printf("Enter choice: "); choice = readInt();

        if (choice == 0) {
            printf("Goodbye!\n");
            return 0;
        } else if (choice == 1) {
            if (loginUser()) {
                break;   /* logged in - move on to the app */
            }
        } else if (choice == 2) {
            registerUser();
        }
    }

    startupAlerts();
    mainMenu();

    return 0;
}
