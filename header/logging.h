#define RESET   "\x1B[0m"
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"
#define TO_RED(x) RED x RESET
#define TO_GREEN(x) GREEN x RESET
#define TO_YELLOW(x) YELLOW x RESET
#define TO_BLUE(x) BLUE x RESET

void redPrint(const char *format, ...);
void greenPrint(const char *format, ...);
void yellowPrint(const char *format, ...);
void bluePrint(const char *format, ...);
