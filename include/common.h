#ifndef COMMON_H
#define COMMON_H

/* ============================================================
   COMMON.H - shared includes, sizes, and compiler-compatibility
   settings used by every module in the project.
   ============================================================ */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#ifdef _MSC_VER
#pragma warning(disable: 4996)   /* allow plain scanf/fopen, no _s versions needed */
#endif

#if defined(__GNUC__)
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif

/* ---------------- Fixed sizes (no dynamic memory anywhere) ---------------- */
#define MAX_MED         50
#define MAX_SUP         20
#define MAX_CUST        50
#define MAX_SALE        100
#define MAX_PUR         100
#define MAX_BILL_ITEMS  20   /* how many different medicines one bill can hold */
#define MAX_USERS       30

#define NAME_LEN        30
#define SHORT_LEN       20
#define MOBILE_LEN      15
#define LOG_LINE_LEN    200

#endif /* COMMON_H */
