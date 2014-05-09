#ifndef _COMMON_H
#define _COMMON_H

#include <stdbool.h>
#include <string.h>

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

/* check if two strings are equal */
#define STR_EQUALS(a, b) (strcmp((a), (b)) == 0)

/* check if string is prior lexicographically */
#define STR_IS_PRIOR(a, b) (strcmp((a), (b)) < 0)

/* allocate a new string and copy to it. return NULL if out of memory */
char* cloneString(const char* str);

/* free an array of pointers */
void freeArray(void** start, int size);

#endif