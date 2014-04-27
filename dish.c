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

Dish dishCreate(char * name, DISH_TYPE type, int sweetness, int sourness, int saltiness, dishResult * result) {
	if (name == NULL) {
		SAFE_ASSIGN(result,DISH_NULL_ARG)
		return NULL;
	}
	Dish dish = (Dish)malloc(sizeof(*dish));
	char * dishName = malloc(strlen(name)+1);
	if ((dish == NULL) || (name == NULL)) {
		SAFE_ASSIGN(result,DISH_OUT_OF_MEMORY)
		free(dish);
		free(name);
		return NULL;
	}
	strcpy(dishName,name);
	dish->name = dishName;
	if (outOfBounds(sweetness,0,10) || 
									outOfBounds(sourness,0,10) ||
									outOfBounds(saltiness,0,10) ||
									outOfBounds(type,0,NUM_DISH_TYPES-1)) {
		SAFE_ASSIGN(result,DISH_BAD_PARAM)
		dishDestroy(dish);
		return NULL;
	}
	dish->sweetness = sweetness;
	dish->sourness = sourness;
	dish->saltiness = saltiness;
	dish->type = type;
	SAFE_ASSIGN(result,DISH_SUCCESS)
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

char * dishGetName (Dish dish) {
	if (dish == NULL) {
		return NULL;
	}
	return dish->name;
}