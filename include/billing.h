#ifndef BILLING_H
#define BILLING_H

extern const char *billFileName;

/* Lets the user pick one or more medicines for a customer (looked up
   by mobile number, must already exist in customer.txt or be added
   on the spot) in a single visit, shows a printed-style bill, reduces
   stock for every item, and saves everything to bills.txt. */
void generateBill(void);

#endif /* BILLING_H */
