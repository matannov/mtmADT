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

void freeArray(void** start, int size) {
	for(int i = 0; i < size; i++) {
		free(*start);
		start++;
	}
	free(start);
}
