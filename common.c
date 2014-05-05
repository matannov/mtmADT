#include <stdlib.h>
#include <string.h>
#include "common.h"

char* cloneString(const char* str) {
	char* temp = malloc(strlen(str)+1);
	if(temp != NULL) {
		strcpy(temp, str);
	}
	return temp;
}
