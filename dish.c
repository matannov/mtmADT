#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "dish.h"
#include "common.h"

#define TYPE_VALUE_MIN 0
#define TYPE_VALUE_MAX (NUM_DISH_TYPES-1)

struct dish{
	char name[DISH_NAME_LENGTH_MAX+1];
	DishType type;
	Taste taste;
};

Dish dishCreate(char const* name, DishType type, Taste taste, DishResult* result) {
	if(name == NULL) {
		ASSIGN_AND_RETURN(result, DISH_NULL_ARGUMENT, NULL)
	}
	if(strlen(name) > DISH_NAME_LENGTH_MAX ||
		!IN_RANGE(taste.sweetness, DISH_TASTE_PARAM_MIN, DISH_TASTE_PARAM_MAX) || 
		!IN_RANGE(taste.sourness, DISH_TASTE_PARAM_MIN, DISH_TASTE_PARAM_MAX) ||
		!IN_RANGE(taste.saltiness, DISH_TASTE_PARAM_MIN, DISH_TASTE_PARAM_MAX) ||
		!IN_RANGE(type, TYPE_VALUE_MIN, TYPE_VALUE_MAX)) {

		ASSIGN_AND_RETURN(result, DISH_BAD_PARAM, NULL)
	}

	Dish dish = malloc(sizeof(*dish));
	if(dish == NULL) {
		ASSIGN_AND_RETURN(result, DISH_OUT_OF_MEMORY, NULL)
	}
	strcpy(dish->name, name);
	dish->taste = taste;
	dish->type = type;
	ASSIGN_AND_RETURN(result, DISH_SUCCESS, dish)
}

Dish dishCopy(Dish source) {
	if(source == NULL) {
		return NULL;
	}
	return dishCreate(source->name, source->type, source->taste, NULL);
}
	
void dishDestroy(Dish dish) {
	free(dish);
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
