#ifndef _DISH_H
#define _DISH_H

typedef enum DISH_TYPE {
	appetizer,
	entree,
	desert,
};

typedef enum dishResult {
	DISH_OUT_OF_MEMORY,
	DISH_SUCCESS
};

typedef struct t_dish {
	DISH_TYPE type;
	int sweetness, sourness, saltyness;
} Dish;

Dish dishCreate(DISH_TYPE type, int sweetness, int sourness, int saltiness, dishResult * result);

#endif // _DISH_H