#ifndef _DISH_H
#define _DISH_H

#define DISH_TASTE_PARAM_MIN 1
#define DISH_TASTE_PARAM_MAX 10

#define DISH_TYPE_MIN 1
#define DISH_TYPE_MAX (DISH_TYPES_END-1)

typedef enum {
	DISH_APPETIZER = DISH_TYPE_MIN,
	DISH_ENTREE,
	DISH_DESERT,
	DISH_TYPES_END
} DishType;

typedef enum {
	DISH_SUCCESS,
	DISH_OUT_OF_MEMORY,
	DISH_NULL_ARGUMENT,
	DISH_BAD_PARAM
} DishResult;

typedef struct {
	int sweetness, sourness, saltiness;
} Taste;

typedef struct Dish_t *Dish;

/*
 * Create a new dish with given params.
 *
 * @param name Name for new dish. The string is copied.
 * @param type Type of dish.
 * @param taste Taste params of dish
 * @param result Result success or error code.
 * 	If "result" is null, it is ignored and no code is returned.
 *	Error codes: DISH_NULL_ARGUMENT, DISH_BAD_PARAM, DISH_OUT_OF_MEMORY
 * @return The new dish, NULL in case of error.
 */
Dish dishCreate(char const* name, DishType type, Taste taste, DishResult* result);

/*
 * Destroy given dish.
 *
 * @param dish Dish to destroy, if NULL does nothing.
 */
void dishDestroy(Dish dish);

/*
 * Create a copy of an existing dish.
 *
 * @param source The dish to copy.
 * @return Dish copy, NULL in case of NULL argument or out of memory.
 */
Dish dishCopy(Dish source);

/*
 * Get dish name.
 *
 * @param dish Dish to get it's name
 * @param name String pointer will be written here. 
 * 	User is responsible to free this memory.
 * @return Result success or error code.
 *	Error codes: DISH_NULL_ARGUMENT, DISH_OUT_OF_MEMORY
 */
DishResult dishGetName(Dish dish, char** name);

/*
 * Get dish taste params.
 *
 * @param dish Dish to read it's params
 * @param taste Taste params will be written here.
 * @return Result success or error code.
 *	Error codes: DISH_NULL_ARGUMENT
 */
DishResult dishGetTaste(Dish dish, Taste* taste);

/*
 * Get dish type.
 *
 * @param dish Dish to read it's type
 * @param type Dish type will be written here.
 * @return Result success or error code.
 *	Error codes: DISH_NULL_ARGUMENT
 */
DishResult dishGetType(Dish dish, DishType* type);


#endif // _DISH_H