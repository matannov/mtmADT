#ifndef _DISH_H
#define _DISH_H

#define DISH_NAME_LENGTH_MAX 63
#define DISH_TASTE_PARAM_MIN 1
#define DISH_TASTE_PARAM_MAX 10

typedef enum {
	APPETIZER,
	ENTREE,
	DESERT,
	NUM_DISH_TYPES
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

typedef struct dish* Dish;

/*
 * Create a new dish with given params.
 *
 * In case any param is invalid or if run out of memory, 
 * an error code is set in "result".
 * If "result" is null, no error code is returned.
 *
 * @param name Name for new dish. The string is copied.
 * @param type Type of dish.
 * @param taste Taste params of dish
 * @param result Result success or error code.
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
 * @return Dish copy, NULL in case of an error.
 */
Dish dishCopy(Dish source);

/*
 * Get dish name.
 *
 * @param dish Dish to get it's name
 * @param buffer Name will be written here. Make sure there is enough space.
 * @return Result success or error code.
 */
DishResult dishGetName(Dish dish, char* buffer);

/*
 * Get dish taste params.
 *
 * @param dish Dish to read it's params
 * @param taste Taste params will be written here.
 * @return Result success or error code.
 */
DishResult dishGetTaste(Dish dish, Taste* taste);

/*
 * Get dish type.
 *
 * @param dish Dish to read it's type
 * @param type Dish type will be written here.
 * @return Result success or error code.
 */
DishResult dishGetType(Dish dish, DishType* type);

/*
 * Get length of dish's name.
 *
 * @param dish Dish to read it's sweetness
 * @param length length of the dish's name will be written here.
 * @return Result success or error code.
 */
 
 DishResult dishGetNameLength(Dish dish, int * length);


#endif // _DISH_H