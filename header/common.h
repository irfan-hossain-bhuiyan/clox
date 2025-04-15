#ifndef clox_common_h
#define clox_common_h
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define DEBUG
#ifdef DEBUG
#define _Enum_assert(ENUM_LAST_MEMBER,ENUM_SIZE) \
		_Static_assert(ENUM_LAST_MEMBER==ENUM_SIZE,#ENUM_LAST_MEMBER "function didn't got updated.")
#else
#define _Enum_assert(ENUM_LAST_MEMBER,ENUM_SIZE)
#endif
// #define DEBUG_PRINT_CODE
//#define DEBUG_TRACE_EXECUTION
#endif
