#ifndef _DISH_H
#define _DISH_H

#define DISH_TASTE_VALUE_MIN 1
#define DISH_TASTE_VALUE_MAX 10

typedef enum {
	APPETIZER,
	ENTREE,
	DESERT,
} DishType;

typedef enum {
	DISH_OUT_OF_MEMORY,
	DISH_NULL_ARGUMENT,
	DISH_BAD_PARAM,
	DISH_SUCCESS
} DishResult;

typedef struct {
	DishType type;
	int sweetness, sourness, saltiness;
} * Dish;


/********************************
	dishCreate
	takes 
********************************/

Dish dishCreate(DishType type, int sweetness, int sourness, int saltiness, DishResult * result);
void dishDestroy(Dish dish);
Dish dishCopy(Dish source, DishResult * result);

#endif // _DISH_H