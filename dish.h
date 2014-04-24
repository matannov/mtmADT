#ifndef _DISH_H
#define _DISH_H

#define NUM_DISH_TYPES 3

typedef enum DISH_TYPE {
	appetizer,
	entree,
	desert,
} DISH_TYPE;

typedef enum dishResult {
	DISH_OUT_OF_MEMORY,
	DISH_BAD_PARAM,
	DISH_SUCCESS
} dishResult;

typedef struct t_dish {
	DISH_TYPE type;
	int sweetness, sourness, saltiness;
} Dish;


/********************************
	dishCreate
	takes 
********************************/

Dish dishCreate(DISH_TYPE type, int sweetness, int sourness, int saltiness, dishResult * result);

#endif // _DISH_H