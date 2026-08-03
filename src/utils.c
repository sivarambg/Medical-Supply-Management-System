#include "common.h"
#include "utils.h"

const char *logFileName = "medistore_log.txt";

/* Appends one line of text to the log file. Uses "a" (append) mode
   so every call adds a new line without erasing old data. */
void writeLog(const char *text) {
    FILE *fp;
    fp = fopen(logFileName, "a");
    if (fp != NULL) {
        fprintf(fp, "%s\n", text);
        fclose(fp);
    }
}

/* Gets today's date using the standard library (time.h). */
void getCurrentDate(int *day, int *month, int *year) {
    time_t t;
    const struct tm *now;
    t = time(NULL);
    now = localtime(&t);
    *day = now->tm_mday;
    *month = now->tm_mon + 1;
    *year = now->tm_year + 1900;
}

int readInt(void) {
    int value;
    int result;
    int ch;

    result = scanf("%d", &value);
    if (result != 1) {
        value = 0;   /* EOF or non-numeric input: fall back to a safe default */
    }

    /* discard the rest of the line so leftover text is never re-read
       by the next input call - this is what stops a bad entry (or a
       closed input stream) from causing an infinite loop */
    ch = getchar();
    while (ch != '\n' && ch != EOF) {
        ch = getchar();
    }

    return value;
}

int readWord(char *buf, int maxlen) {
    char fmt[16];
    int result;

    if (maxlen <= 1) {
        buf[0] = '\0';
        return 0;
    }

    /* build a width-limited "%Ns" format at runtime so the read can
       never overflow buf, no matter what maxlen is passed in */
    sprintf(fmt, "%%%ds", maxlen - 1);
    result = scanf(fmt, buf);

    if (result != 1) {
        strcpy(buf, "0");   /* EOF/failure: behave like the user typed the "stop" sentinel */
        return 0;
    }
    return 1;
}
