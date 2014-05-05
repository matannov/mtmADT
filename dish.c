#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "dish.h"
#include "common.h"

struct dish {
	char* name;
	DishType type;
	Taste taste;
};

Dish dishCreate(char const* name, DishType type, Taste taste, DishResult* result) {
	if(name == NULL) {
		ASSIGN_AND_RETURN(result, DISH_NULL_ARGUMENT, NULL)
	}
	if(!IN_RANGE(taste.sweetness, DISH_TASTE_PARAM_MIN, DISH_TASTE_PARAM_MAX) || 
		!IN_RANGE(taste.sourness, DISH_TASTE_PARAM_MIN, DISH_TASTE_PARAM_MAX) ||
		!IN_RANGE(taste.saltiness, DISH_TASTE_PARAM_MIN, DISH_TASTE_PARAM_MAX) ||
		!IN_RANGE(type, DISH_TYPE_MIN, DISH_TYPE_MAX)) {

		ASSIGN_AND_RETURN(result, DISH_BAD_PARAM, NULL)
	}

	Dish dish = malloc(sizeof(*dish));
	if(dish == NULL) {
		ASSIGN_AND_RETURN(result, DISH_OUT_OF_MEMORY, NULL)
	}
	dish->name = cloneString(name);
	if(dish->name == NULL) {
		dishDestroy(dish);
		ASSIGN_AND_RETURN(result, DISH_OUT_OF_MEMORY, NULL)
	}
	dish->type = type;
	dish->taste = taste;
	ASSIGN_AND_RETURN(result, DISH_SUCCESS, dish)
}
	
void dishDestroy(Dish dish) {
	if(dish == NULL) {
		return;
	}
	free(dish->name);
	free(dish);
}

Dish dishCopy(Dish source) {
	if(source == NULL) {
		return NULL;
	}
	return dishCreate(source->name, source->type, source->taste, NULL);
}

DishResult dishGetName(Dish dish, char* buffer) {
	if(buffer == NULL || dish == NULL) {
		return DISH_NULL_ARGUMENT;
	}
	strcpy(buffer, dish->name);
	return DISH_SUCCESS;
}

DishResult dishGetTaste(Dish dish, Taste* taste) {
	if(dish == NULL) {
		return DISH_NULL_ARGUMENT;
	}
	*taste = dish->taste;
	return DISH_SUCCESS;
}

DishResult dishGetType(Dish dish, DishType* type) {
	if(dish == NULL) {
		return DISH_NULL_ARGUMENT;
	}
	*type = dish->type;
	return DISH_SUCCESS;
}
