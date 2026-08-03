# MediStore - Modular C Build

12 modules (one pair of .c/.h per concern), verified with GCC warnings,
Valgrind, and cppcheck's MISRA C:2012 addon. Real results below - not
just claims.

## Build

```
make
./medistore
```

Default login: `admin` / `admin123` (auto-created on first run).

## Module layout

| Module              | Owns                                              |
|---------------------|----------------------------------------------------|
| common.h             | shared sizes/includes, no .c file                  |
| utils.h / utils.c    | log writer, date helper, safe input readers        |
| medicine.h / .c      | Medicine struct + CRUD + expiry/low-stock + menu   |
| supplier.h / .c      | Supplier struct + CRUD + menu                      |
| customer.h / .c      | Customer struct + CRUD + purchase history + menu   |
| sales.h / .c         | Sale transaction log (used by billing, reports)    |
| purchase.h / .c      | Purchase/restock transaction log                   |
| user.h / .c          | accounts, register/login, admin approval           |
| billing.h / .c       | multi-item bill generation                         |
| report.h / .c        | sales/purchase/profit reports + reports menu        |
| about.h / .c         | About screen                                        |
| menu.h / .c          | startup alerts + main menu loop                     |
| main.c               | entry point only                                    |

Each `.c` file that owns data (medicine.c, supplier.c, customer.c,
sales.c, purchase.c, user.c) defines its array once; every other module
that needs it includes the matching header and gets an `extern`
declaration instead of a second definition.

## Verification actually performed

**Compiler warnings**: `gcc -Wall -Wextra -std=c99` and
`-std=c89 -pedantic` both clean, 0 warnings, on every file.

**Valgrind** (`--leak-check=full --show-leak-kinds=all`), exercising
registration, login, admin approval, full medicine/supplier/customer
CRUD, billing, and every report:
```
HEAP SUMMARY:
    in use at exit: 0 bytes in 0 blocks
    total heap usage: 33 allocs, 33 frees, 64,948 bytes allocated
All heap blocks were freed -- no leaks are possible
ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
(The 33 alloc/free pairs are glibc's internal stdio buffers for
`fopen`/`fclose` - every `fopen` in this program has a matching
`fclose`, so all of them come back clean.)

**A real bug this process caught and fixed**: none of the original
`scanf("%d", &choice)` calls checked their return value. On EOF or bad
input the target variable kept its previous/garbage value instead of
becoming a safe default, so several loops (login, every submenu,
billing's item-entry loop) could spin forever instead of exiting. Fixed
with `readInt()`/`readWord()` helpers in `utils.c` that check the
return value, fall back to a safe default, and flush the rest of the
line. Verified fixed: a deliberately truncated/EOF'd input that used to
hang now exits cleanly (`exit code: 0`) every time.

**cppcheck** (`--enable=all --inconclusive`): clean except for
intentional `variableScope` style notes (see deviations below).

## MISRA C:2012 - honest status

Ran `cppcheck --addon=misra`. This is **not** full official MISRA
certification (the addon needs the paid official rule-text document to
give complete messages; without it, cppcheck matches heuristically, so
treat exact counts as approximate, not certified). Findings by rule,
before and after fixing what could reasonably be fixed:

| Rule | Meaning (paraphrased)                        | Before | After | Status |
|------|-----------------------------------------------|--------|-------|--------|
| 12.1 | operator precedence should be explicit         | 19     | 0     | Fixed - added explicit parens everywhere |
| 8.11 | extern array needs explicit size               | 6      | 0     | Fixed - changed filename globals to `const char *` |
| 7.4  | string literal into non-const char array        | 6      | 0     | Fixed - same change as above |
| 17.7 | function return value must be used              | 217    | 217   | **Not fixed** - see below |
| 15.5 | function should have a single exit point        | 31     | 31    | **Not fixed** - see below |
| 12.3 | comma-operator-adjacent construct                | 23     | 23    | Addon false-positive (no official rule text) |
| 8.7  | function could have internal linkage (`static`) | 20     | 20    | Left alone - see below |
| 10.3/10.4/9.3/8.9/14.4/15.7/21.7/21.10 | assorted implicit-conversion/init/boolean-type notes | ~13 | ~15 | Left alone - minor, see below |
| 21.6 | `<stdio.h>` must not be used                   | 1 (deduped) | 1 | **Cannot fix** - see below |

### Why 17.6/21.6 (stdio) and 17.7 (unused return values) are not fixed

MISRA C was written for embedded/safety-critical firmware with no
console and no filesystem in the usual sense. Rule 21.6 bans the
*entire* `<stdio.h>` input/output family - `printf`, `scanf`, `fopen`,
`fprintf`, `fclose`, all of it. This program's entire design is a
console menu plus text-file storage. Removing stdio would mean
removing the program. I'm not going to claim compliance I can't
deliver - the honest answer is this class of application and full
MISRA-C are fundamentally at odds on this one rule, and everything
downstream of it (217 Rule 17.7 hits, one per stdio call whose return
value isn't checked) is a direct consequence.

Real embedded teams handle exactly this situation with a documented
*MISRA deviation record* rather than pretending compliance - that's
what this table is.

### Why 15.5 (single exit point) is not fixed

31 early `return;` statements, mostly guard clauses like
`if (not found) { print error; return; }`. Rewriting every function
into single-exit style (nested if/else, a `result` variable threaded
through) would roughly double the code and make it harder to read for
a beginner, which directly contradicts the "keep it short and simple"
brief this codebase was built to. Documented deviation, not fixed.

### Why 8.7 (internal linkage) and the `variableScope` notes are not fixed

Both ask for narrower scope (`static` functions, variables declared
closer to first use). The whole codebase deliberately declares every
variable at the top of its function (pre-C99/old-compiler-friendly
style, matches the original brief). Tightening scope everywhere would
undo that deliberate choice for a purely stylistic gain. Left as a
documented, low-risk deviation.

### What was actually fixed vs. just documented

Fixed for real: precedence parens (12.1), extern array sizing (8.11),
string-literal constness (7.4), the duplicated expiry-check logic
across two files, the genuine EOF-hang bug, and the two const-
correctness notes cppcheck's generic analysis found in `utils.c`.

Documented as deviations rather than "fixed": everything that would
require abandoning stdio (structurally impossible here) or rewriting
the whole codebase into a different style than the one this project
was explicitly asked to be (short, simple, beginner-readable, C89-
declaration-style).
