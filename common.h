#ifndef _COMMON_H
#define _COMMON_H

#include <stdbool.h>

typedef enum {SUCCESS, FAILURE} Result;

/* assign only if ptr is not NULL */
#define SAFE_ASSIGN(ptr, result) \
	if ((ptr) != NULL) { \
		*(ptr) = (result); \
	}

/* assign result to ptr and return value */
#define ASSIGN_AND_RETURN(ptr, result, value) \
		SAFE_ASSIGN(ptr, result) \
		return (value);

/* check value in range */
#define IN_RANGE(value, min, max) \
	(((value) >= (min) && (value) <= (max)) ? true : false)

/* allocate a new string and copy to it. return FAILURE if out of memory */
Result cloneString(char** strNew, const char* str);

#endif