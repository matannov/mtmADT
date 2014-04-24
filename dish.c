#include "dish.h"
#include "stdbool.h"
#include "stdlib.h"

static bool outOfBounds(int value, int min, int max) {
	if ((value < min) || (value > max)) { 
		return true; 
	}
	else {
		return false;
	}
}

Dish dishCreate(DISH_TYPE type, int sweetness, int sourness, int saltiness, dishResult * result) {
	Dish dish = (Dish)malloc(sizeof(*dish));
	if (dish == NULL) {
		if (result != NULL) {
			*result = DISH_OUT_OF_MEMORY;
			}
		return dish;
	}
	if (outOfBounds(sweetness,0,10) || 
									outOfBounds(sourness,0,10) ||
									outOfBounds(saltiness,0,10) ||
									outOfBounds(type,0,NUM_DISH_TYPES-1)) {
		 if (result != NULL) {
			*result = DISH_BAD_PARAM;
		}
		return dish;
	}
	dish->sweetness = sweetness;
	dish->sourness = sourness;
	dish->saltiness = saltiness;
	dish->type = type;
	if (result != NULL) {
		*result = DISH_SUCCESS;
	}
	return dish;
}

Dish dishCopy(Dish source, dishResult * result) {
	if (source == NULL) {
		if (result != NULL) {
			*result = DISH_NULL_ARGUMENT;
		}
		return NULL;
	}
	return(dishCreate(source->type,source->sweetness,source->sourness,
						source->saltiness,result));
	}
	
void dishDestroy(Dish dish) {
	free(dish);
}