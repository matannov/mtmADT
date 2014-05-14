#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "priority_queue.h"
#include "chef.h"

struct chef {
	char* name;
	PriorityQueue dishes;
	int points;
};

/* wrap dishCopy for use in priority queue */
static PriorityQueueElement copyDish(PriorityQueueElement dish) {
	return dishCopy(dish);
}

/* wrap dishDestroy for use in priority queue */
static void destroyDish(PriorityQueueElement dish) {
	dishDestroy(dish);
}

Chef chefCreate(char const* name, ChefResult* result) {
	if(name == NULL) {
		ASSIGN_AND_RETURN(result, CHEF_NULL_ARGUMENT, NULL)
	}
	Chef chef = malloc(sizeof(*chef));
	if(chef == NULL) {
		ASSIGN_AND_RETURN(result, CHEF_OUT_OF_MEMORY, NULL)
	}
	chef->name = cloneString(name);
	chef->dishes = priorityQueueCreate(&copyDish, &destroyDish);
	if(chef->name ==  NULL || chef->dishes == NULL) {
		chefDestroy(chef);
		ASSIGN_AND_RETURN(result, CHEF_OUT_OF_MEMORY, NULL)
	}
	ASSIGN_AND_RETURN(result, CHEF_SUCCESS, chef)
}

void chefDestroy(Chef chef) {
	if(chef == NULL) {
		return;
	}
	priorityQueueDestroy(chef->dishes);
	free(chef->name);
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
	if(copy->dishes == NULL) {
		chefDestroy(copy);
		return NULL;
	}
	return copy;
}

ChefResult chefGetName(Chef chef, char** name) {
	if(chef == NULL || name == NULL) {
		return CHEF_NULL_ARGUMENT;
	}
	*name = cloneString(chef->name);
	if(*name == NULL) {
		return CHEF_OUT_OF_MEMORY;
	}
	return CHEF_SUCCESS;
}

ChefResult chefAddDish(Chef chef, Dish dish, int priority) {
	if(chef == NULL) {
		return CHEF_NULL_ARGUMENT;
	}
	if(priority < CHEF_DISH_PRIORITY_MIN) {
		return CHEF_BAD_PRIORITY;
	}
	PriorityQueueResult result = priorityQueueAdd(chef->dishes, &dish, priority);
	if(result == PRIORITY_QUEUE_OUT_OF_MEMORY) {
		return CHEF_OUT_OF_MEMORY;
	}
	return CHEF_SUCCESS;
}

ChefResult chefGetTopDishName(Chef chef, char** name) {
	if(chef == NULL || name == NULL) {
		return CHEF_NULL_ARGUMENT;
	}
	Dish dish = priorityQueueTop(chef->dishes);
	if(dish == NULL) {
		return CHEF_HAS_NO_DISHES;
	}
	if(dishGetName(dish, name) == DISH_OUT_OF_MEMORY) {
		return CHEF_OUT_OF_MEMORY;
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

ChefResult chefCompareNames(Chef first, Chef second, int* result) {
	if(first == NULL || second == NULL || result == NULL) {
		return CHEF_NULL_ARGUMENT;
	}
	*result = strcmp(first->name, second->name);
	return CHEF_SUCCESS;
}

ChefResult chefIsBetterRanked(Chef first, Chef second, bool* firstBetter) {
	if(first == NULL || second == NULL) {
		return CHEF_NULL_ARGUMENT;
	}
	*firstBetter = (first->points > second->points);
	return CHEF_SUCCESS;
}

bool chefHasDish(Chef chef) {
	if(chef == NULL) {
		return false;
	}
	return (priorityQueueGetSize(chef->dishes) != 0);
}

ChefResult chefTakeTopDish(Chef chef, Dish* dish) {
	if(chef == NULL || dish == NULL) {
		return CHEF_NULL_ARGUMENT;
	}
	Dish tempDish = priorityQueueTop(chef->dishes);
	if(tempDish == NULL) {
		return CHEF_HAS_NO_DISHES;
	}
	*dish = dishCopy(tempDish);
	if(*dish == NULL) {
		return CHEF_OUT_OF_MEMORY;
	}
	priorityQueuePop(chef->dishes);
	return CHEF_SUCCESS;
}

ChefResult chefGivePoint(Chef chef) {
	if(chef == NULL) {
		return CHEF_NULL_ARGUMENT;
	}
	chef->points++;
	return CHEF_SUCCESS;
}
