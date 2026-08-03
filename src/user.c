#include "common.h"
#include "user.h"
#include "utils.h"

struct User users[MAX_USERS];
int userCount = 0;
int currentUserIsAdmin = 0;

const char *userFileName = "users.txt";

/* ============================================================
   FILE STORAGE
   ============================================================ */

void saveUser(struct User u) {
    FILE *fp;
    fp = fopen(userFileName, "a");   /* append = add this one account to the end */
    if (fp != NULL) {
        fprintf(fp, "%d,%s,%s,%d,%d\n", u.id, u.username, u.password, u.isAdmin, u.isApproved);
        fclose(fp);
    }
}

void saveAllUsers(void) {
    FILE *fp;
    int i;
    fp = fopen(userFileName, "w");   /* rewrite = used when an account's approval status changes */
    if (fp == NULL) {
        return;
    }
    for (i = 0; i < userCount; i++) {
        fprintf(fp, "%d,%s,%s,%d,%d\n", users[i].id, users[i].username, users[i].password, users[i].isAdmin, users[i].isApproved);
    }
    fclose(fp);
}

void loadUsers(void) {
    FILE *fp;
    struct User u;
    struct User defaultAdmin;

    userCount = 0;
    fp = fopen(userFileName, "r");
    if (fp != NULL) {
        while (fscanf(fp, "%d,%19[^,],%19[^,],%d,%d\n", &u.id, u.username, u.password, &u.isAdmin, &u.isApproved) == 5) {
            if (userCount < MAX_USERS) {
                users[userCount] = u;
                userCount++;
            }
        }
        fclose(fp);
    }

    /* very first run ever: there are no accounts yet, so create one
       default admin account. Otherwise nobody could ever log in to
       approve anybody else! */
    if (userCount == 0) {
        defaultAdmin.id = 1;
        strcpy(defaultAdmin.username, "admin");
        strcpy(defaultAdmin.password, "admin123");
        defaultAdmin.isAdmin = 1;
        defaultAdmin.isApproved = 1;

        users[0] = defaultAdmin;
        userCount = 1;
        saveUser(defaultAdmin);

        printf("No accounts found - created a default admin account.\n");
        printf("(username: admin, password: admin123 - change this once you can!)\n");
    }
}

/* ============================================================
   AUTHENTICATION
   ============================================================ */

/* New sign-ups always come in as a normal (non-admin) account that is
   NOT yet approved - an admin has to approve them before they can log in. */
void registerUser(void) {
    struct User u;
    int i, alreadyExists;

    if (userCount >= MAX_USERS) {
        printf("User list is full!\n");
        return;
    }

    printf("Choose a username: "); scanf("%19s", u.username);

    alreadyExists = 0;
    for (i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, u.username) == 0) {
            alreadyExists = 1;
        }
    }
    if (alreadyExists) {
        printf("That username is already taken. Try logging in instead.\n");
        return;
    }

    printf("Choose a password: "); scanf("%19s", u.password);

    u.id = userCount + 1;
    u.isAdmin = 0;
    u.isApproved = 0;

    users[userCount] = u;
    userCount++;
    saveUser(u);

    writeLog("REGISTER USER performed (pending approval)");
    printf("Registration submitted! An admin must approve your account before you can log in.\n");
}

/* Returns 1 if login succeeds, 0 otherwise. Also sets currentUserIsAdmin. */
int loginUser(void) {
    char username[SHORT_LEN], password[SHORT_LEN];
    int attempts, i, matched;

    for (attempts = 0; attempts < 3; attempts++) {
        printf("Username: "); scanf("%19s", username);
        printf("Password: "); scanf("%19s", password);

        matched = -1;
        for (i = 0; i < userCount; i++) {
            if ((strcmp(users[i].username, username) == 0) && (strcmp(users[i].password, password) == 0)) {
                matched = i;
            }
        }

        if (matched == -1) {
            printf("Invalid login. Try again.\n");
            continue;
        }

        if (users[matched].isApproved == 0) {
            printf("Your account is pending admin approval. Please try again later.\n");
            return 0;   /* correct credentials, but not allowed in yet - don't keep retrying */
        }

        currentUserIsAdmin = users[matched].isAdmin;
        printf("Login successful! Welcome, %s.\n", username);
        return 1;
    }
    return 0;
}

/* Admin-only screen: shows every pending account and lets the admin
   approve them one at a time by username. */
void approvePendingUsers(void) {
    char uname[SHORT_LEN];
    int i, found;

    if (!currentUserIsAdmin) {
        printf("Access denied. Admins only.\n");
        return;
    }

    do {
        printf("\n--- Pending Users ---\n");
        found = 0;
        for (i = 0; i < userCount; i++) {
            if (users[i].isApproved == 0) {
                printf("%d) %s\n", users[i].id, users[i].username);
                found = 1;
            }
        }
        if (!found) {
            printf("No pending users.\n");
            return;
        }

        printf("Enter username to approve (0 to stop): ");
        readWord(uname, sizeof(uname));
        if (strcmp(uname, "0") == 0) {
            return;
        }

        for (i = 0; i < userCount; i++) {
            if (strcmp(users[i].username, uname) == 0) {
                users[i].isApproved = 1;
                saveAllUsers();   /* rewrite users.txt with the updated approval status */
                writeLog("APPROVE USER performed");
                printf("User '%s' approved!\n", uname);
            }
        }
    } while (1);
}
