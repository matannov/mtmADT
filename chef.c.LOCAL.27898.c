#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "priority_queue.h"
#include "chef.h"

struct chef{
	char name[CHEF_NAME_LENGTH_MAX+1];
	PriorityQueue dishes;
	int points;
};
	
/* wrap dishCopy for use in priority queue */
static PriorityQueueElement copyDish(PriorityQueueElement dish) {
	return (PriorityQueueElement)dishCopy((Dish)dish);
}

/* wrap dishDestroy for use in priority queue */
static void destroyDish(PriorityQueueElement dish) {
	dishDestroy((Dish)dish);
}

Chef chefCreate(char const* name, ChefResult* result) {
	if(name == NULL) {
		ASSIGN_AND_RETURN(result, CHEF_NULL_ARGUMENT, NULL)
	}
	if(strlen(name) > CHEF_NAME_LENGTH_MAX) {
		ASSIGN_AND_RETURN(result, CHEF_BAD_PARAM, NULL)
	}

	Chef chef = malloc(sizeof(*chef));
	if(chef == NULL) {
		ASSIGN_AND_RETURN(result, CHEF_OUT_OF_MEMORY, NULL)
	}
	chef->dishes = priorityQueueCreate(&copyDish, &destroyDish);
	if(chef->dishes == NULL) {
		chefDestroy(chef);
		ASSIGN_AND_RETURN(result, CHEF_OUT_OF_MEMORY, NULL)
	}
	strcpy(chef->name, name);
	ASSIGN_AND_RETURN(result, CHEF_SUCCESS, chef)
}

void chefDestroy(Chef chef) {
	if(chef == NULL) {
		return;
	}
	priorityQueueDestroy(chef->dishes);
	free(chef);
}

Chef chefCopy(Chef source) {
	if(source == NULL) {
		return NULL;
	}
	Chef copy = chefCreate(source->name, NULL);
	if(copy == NULL) {
		return NULL;
	}
	priorityQueueDestroy(copy->dishes);
	copy->dishes = priorityQueueCopy(source->dishes);
	return copy;
}

ChefResult chefGetNameLength(Chef chef, int* nameLength) {
	if ((chef == NULL) || (nameLength == NULL)) {
		return CHEF_NULL_ARGUMENT;
	}
	*nameLength = strlen(chef->name);
	return CHEF_SUCCESS;
}

ChefResult chefGetName(Chef chef, char* buffer) {
	if(buffer == NULL || chef == NULL) {
		return CHEF_NULL_ARGUMENT;
	}
	strcpy(buffer, chef->name);
	return CHEF_SUCCESS;
}

ChefResult chefAddDish(Chef chef, Dish dish, int priority) {
	if(chef == NULL) {
		return CHEF_NULL_ARGUMENT;
	}
	if(priority < DISH_PRIORITY_MIN) {
		return CHEF_BAD_PRIORITY;
	}
	PriorityQueueResult result = priorityQueueAdd(chef->dishes, &dish, priority);
	if(result == PRIORITY_QUEUE_OUT_OF_MEMORY) {
		return CHEF_OUT_OF_MEMORY;
	}
	return CHEF_SUCCESS;
}

ChefResult chefGetTopDish(Chef chef, char* buffer) {
	if(buffer == NULL || chef == NULL) {
		return CHEF_NULL_ARGUMENT;
	}
	Dish dish = priorityQueueTop(chef->dishes);
	
	if (dishGetName(dish,buffer) == DISH_NULL_ARGUMENT) {
		return CHEF_HAS_NO_DISHES;
	}
	return CHEF_SUCCESS;
}

ChefResult chefGetPoints(Chef chef, int* points) {
	if(points == NULL || chef == NULL) {
		return CHEF_NULL_ARGUMENT;
	}
	*points = chef->points;
	return CHEF_SUCCESS;
}

ChefResult chefIsBetterRanked(Chef first, Chef second, bool* firstBetter) {
	if(first == NULL || second == NULL) {
		return CHEF_NULL_ARGUMENT;
	}
	*firstBetter = (first->points > second->points ||
			(first->points == second->points && 
			strcmp(first->name, second->name) > 0));
	return CHEF_SUCCESS;
}

/*
ChefResult isSameChef(Chef first, Chef second, bool* areIdentical) {
	if(first == NULL || second == NULL || areIdentical == NULL) {
		return CHEF_NULL_ARGUMENT;
	}
	*areIdentical = (strcmp(first->name,second->name) == 0);
	return CHEF_SUCCESS;
}
*/
