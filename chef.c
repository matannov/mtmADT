#include "chef.h"

// move macros to header

#define chefCreate_return(error,chef) \
	if (result != NULL) {	\
			*result = error;	\
		}	\
	return chef;

Chef chefCreate(char * const name, chefResult * result) {
	Chef chef = (Chef)malloc(sizeof(*chef));
	if (chef == NULL) {
		chefCreate_return(CHEF_OUT_OF_MEMORY,chef)
	}
	if (name == NULL) {
		chefCreate_return(CHEF_NULL_ARGUMENT,chef)
	}
	chef.name = (char*)malloc(sizeof(char)*strlen(name));
	if (chef.name == NULL) {
		chefCreate_return(CHEF_OUT_OF_MEMORY,chef)
	}
	strcpy(chef.name,name);
	chefCreate_return(CHEF_SUCCESS,chef)
}

void chefDestroy(Chef chef) {
	if (chef == NULL) {
		return;
	}
	free(chef.name);
	priorityQueueDestroy(chef.dishes);
	free(chef);
}

chefResult chefAddDish(Dish dish, Chef chef, int priority) {
	if (chef == NULL) {
		return CHEF_NULL_ARGUMENT;
	}
	if (priority < 0) {
		return CHEF_BAD_PRIORITY;
	}
	priorityQueueAdd(chef.dishes
}

chefResult isSameChef(Chef first, Chef second, bool * chefsAreIdentical) {
	
}