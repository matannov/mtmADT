#include <stdlib.h>
#include <string.h>
#include "common.h"

Result cloneString(char** strNew, const char* str) {
	char* temp = malloc(strlen(str)+1);
	if(temp == NULL) {
		return FAILURE;
	}
	strcpy(temp, str);
	*strNew = temp;
	return SUCCESS;
}
