#include "chef.h"
#include <string.h>
#include <stdlib.h>

// move macros to header

#define chefCreate_return(error,chef) \
	if (result != NULL) {	\
			*result = error;	\
		}	\
	return chef;
	
static PriorityQueueElement copyDish(PriorityQueueElement dish) {
	dishResult result;
	PriorityQueueElement copy = (PriorityQueueElement)(dishCopy((Dish)dish,&result));
	if (result != DISH_SUCCESS) {
		return NULL;
	}
	return copy;
}

static void destroyDish(PriorityQueueElement dish) {
	dishDestroy((Dish)dish);
}

Chef chefCreate(char * const name, ChefResult * result) {
	Chef chef = (Chef)malloc(sizeof(*chef));
	if (chef == NULL) {
		chefCreate_return(CHEF_OUT_OF_MEMORY,chef)
	}
	if (name == NULL) {
		chefCreate_return(CHEF_NULL_ARGUMENT,chef)
	}
	chef->name = (char*)malloc(sizeof(char)*strlen(name));
	if (chef->name == NULL) {
		chefCreate_return(CHEF_OUT_OF_MEMORY,chef)
	}
	strcpy(chef->name,name);
	chef->dishes = priorityQueueCreate(&copyDish,&destroyDish);
	chefCreate_return(CHEF_SUCCESS,chef)
}

void chefDestroy(Chef chef) {
	if (chef == NULL) {
		return;
	}
	free(chef->name);
	priorityQueueDestroy(chef->dishes);
	free(chef);
}

ChefResult chefAddDish(Dish dish, Chef chef, int priority) {
	if (chef == NULL) {
		return CHEF_NULL_ARGUMENT;
	}
	if (priority < 1) {
		return CHEF_BAD_PRIORITY;
	}
	if (priorityQueueAdd(chef->dishes,(void*)(&dish),priority) == PRIORITY_QUEUE_OUT_OF_MEMORY) {
		return CHEF_OUT_OF_MEMORY;
	}
	return CHEF_SUCCESS;
}

bool chefIsBetter(Chef first, Chef second) {
	if ((first == NULL) || (second == NULL)) {
		return false;
	}
	if (first.points > second.points) {
		return true;
	}
	else if ((first.points == second.points) && (strcmp(first.name,second.name) > 0)) { // add chefGetName?
		return true;
	}
	else {
		return false;
	}
}

ChefResult isSameChef(Chef first, Chef second, bool * chefsAreIdentical) {
	if ((first == NULL) || (second == NULL) || (chefsAreIdentical == NULL)) {
		return CHEF_NULL_ARGUMENT;
	}
	if (strcmp(first->name,second->name) == 0) {
		*chefsAreIdentical = true;
	}
	else {
		*chefsAreIdentical = false;
	}
	return CHEF_SUCCESS;
}