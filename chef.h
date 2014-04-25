#ifndef _CHEF_H
#define _CHEF_H

#include "dish.h"
#include "priority_queue.h"

typedef enum chefResult {
	CHEF_OUT_OF_MEMORY,
	CHEF_NULL_ARGUMENT,
	CHEF_BAD_PRIORITY,
	CHEF_SUCCESS
} chefResult;

typedef struct t_chef {
	char * name;
	PriorityQueue dishes;
	int points;
} * Chef;

Chef chefCreate(char * const name, chefResult * result);
void chefDestroy(Chef chef);
chefResult chefAddDish(Dish dish, Chef chef, int priority);

/********************************
	isSameChef
	takes two Chefs and a pointer to a bool
	returns a chefResult,
	and writes true to the bool if they have the same name,
	and false otherwise
********************************/

chefResult isSameChef(Chef first, Chef second, bool * chefsAreIdentical);

#endif // _CHEF_H