#ifndef _CHEF_H
#define _CHEF_H

#include "dish.h"
#include "priority_queue.h"

typedef enum chefResult {
	CHEF_OUT_OF_MEMORY,
	CHEF_SUCCESS
};

typedef struct t_chef {
	char * name;
	PriorityQueue dishes;
	int points;
} * Chef;

Chef chefCreate(char * const name, chefResult * result);
void chefDestroy(Chef chef);
chefResult chefAddDish(Dish dish, Chef chef);


#endif // _CHEF_H