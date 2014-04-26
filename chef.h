#ifndef _CHEF_H
#define _CHEF_H

#include "dish.h"
#include "priority_queue.h"

typedef enum {
	CHEF_OUT_OF_MEMORY,
	CHEF_NULL_ARGUMENT,
	CHEF_BAD_PRIORITY,
	CHEF_SUCCESS
} ChefResult;

typedef struct t_chef {
	char * name;
	PriorityQueue dishes;
	int points;
} * Chef;

Chef chefCreate(char * const name, ChefResult * result);
void chefDestroy(Chef chef);
ChefResult chefAddDish(Dish dish, Chef chef, int priority);

/********************************
	isSameChef
	takes two Chefs and a pointer to a bool
	returns a chefResult,
	and writes true to the bool if they have the same name,
	and false otherwise
********************************/

ChefResult isSameChef(Chef first, Chef second, bool * chefsAreIdentical);

#endif // _CHEF_H