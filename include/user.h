#ifndef USER_H
#define USER_H

#include "common.h"

/* A login account. isAdmin: 1 = admin, 0 = normal user.
   isApproved: 1 = can log in, 0 = still waiting for admin approval. */
struct User {
    int id;
    char username[SHORT_LEN];
    char password[SHORT_LEN];
    int isAdmin;
    int isApproved;
};

extern struct User users[MAX_USERS];
extern int userCount;
extern int currentUserIsAdmin;   /* set to 1 after an admin logs in successfully */

extern const char *userFileName;

/* ---- File storage ---- */
void saveUser(struct User u);
void saveAllUsers(void);
void loadUsers(void);   /* also bootstraps a default admin on the very first run */

/* ---- Authentication ---- */
void registerUser(void);
int  loginUser(void);              /* returns 1 on success, 0 on failure */
void approvePendingUsers(void);    /* admin-only */

#endif /* USER_H */
