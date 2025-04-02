
#include <stdio.h>
#include <stdarg.h>

#define RESET   "\x1B[0m"
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"

void redPrint(const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf(RED);
    vprintf(format, args);
    printf(RESET);
    va_end(args);
}

void greenPrint(const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf(GREEN);
    vprintf(format, args);
    printf(RESET);
    va_end(args);
}

void yellowPrint(const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf(YELLOW);
    vprintf(format, args);
    printf(RESET);
    va_end(args);
}

void bluePrint(const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf(BLUE);
    vprintf(format, args);
    printf(RESET);
    va_end(args);
}
