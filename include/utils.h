#ifndef UTILS_H
#define UTILS_H

/* Name of the general append-only action log, used by every module. */
extern const char *logFileName;

/* Appends one line of text to the log file (fopen "a" mode). */
void writeLog(const char *text);

/* Gets today's date using the standard library (time.h). */
void getCurrentDate(int *day, int *month, int *year);

/* Reads one integer from stdin. Returns 0 if the input is missing,
   malformed, or the input stream has ended (EOF). Also discards any
   leftover characters on the same line, so a bad entry (or a closed
   input stream) can never leave a menu loop spinning forever waiting
   for a value that will never come. Every "%d" read of a menu choice
   or a loop-controlling number (e.g. "0 to stop") goes through this
   instead of calling scanf("%d", ...) directly. */
int readInt(void);

/* Reads one whitespace-delimited word into buf (max maxlen-1 chars,
   always NUL-terminated). Returns 1 on success, 0 if the input
   stream ended (EOF) - in that case buf is set to "0" so a caller
   using it as a "0 to stop" loop condition exits safely instead of
   spinning forever on a closed input stream. */
int readWord(char *buf, int maxlen);

#endif /* UTILS_H */
