#ifndef _CHEF_H
#define _CHEF_H

#include <stdbool.h>
#include "dish.h"

#define CHEF_DISH_PRIORITY_MIN 0

typedef enum {
	CHEF_SUCCESS,
	CHEF_OUT_OF_MEMORY,
	CHEF_NULL_ARGUMENT,
	CHEF_BAD_PRIORITY,
	CHEF_HAS_NO_DISHES
} ChefResult;

typedef struct chef* Chef;

/*
 * Create a new chef with given name.
 *
 * @param name Name for new chef. The string is copied.
 * @param result Result success or error code.
 *	If "result" is null, no code is returned.
 *	Error codes: CHEF_NULL_ARGUMENT, CHEF_OUT_OF_MEMORY
 * @return The new chef, NULL in case of error.
 */
Chef chefCreate(char const* name, ChefResult* result);

/*
 * Destroy given chef.
 *
 * @param chef Chef to destroy, if NULL does nothing.
 */
void chefDestroy(Chef chef);

/*
 * Create a copy of an existing chef.
 *
 * @param source The chef to copy.
 * @return Chef copy, NULL in case of NULL argument or out of memory.
 */
Chef chefCopy(Chef source);

/*
 * Get chef name.
 *
 * @param chef Chef to get name of
 * @param buffer Name will be written here. Make sure there is enough space.
 * @return Result success or error code.
 *	Error codes: CHEF_NULL_ARGUMENT
 */
ChefResult chefGetName(Chef chef, char* buffer);

/*
 * Add a dish with given priority to chef.
 * 
 * Priority is represented by a non negative integer. Higher priority means
 * the chef likes this dish more relatively to other dishes.
 *
 * @param chef Chef to add dish to
 * @param dish Dish to add. The dish is copied.
 * @param priority Priority of the dish.
 * @return Result success or error code.
 *	Error codes: CHEF_NULL_ARGUMENT, CHEF_BAD_PRIORITY, CHEF_OUT_OF_MEMORY
 */
ChefResult chefAddDish(Chef chef, Dish dish, int priority);

/*
 * Get the name of dish with highest priority. If some dishes have the 
 * same priority, the dish that was added first is returned.
 *
 * @param chef Chef to get the name of top dish from.
 * @param buffer Name will be written here. Make sure there is enough space.
 * @return Result success or error code.
 *	Error codes: CHEF_NULL_ARGUMENT, CHEF_HAS_NO_DISHES
 */
ChefResult chefGetTopDish(Chef chef, char* buffer);

/*
 * Get the number of points the chef has.
 *
 * @param chef Chef to get the points from.
 * @param points Points will be written here.
 * @return Result success or error code.
 *	Error codes: CHEF_NULL_ARGUMENT
 */
ChefResult chefGetPoints(Chef chef, int* points);

/*
 * Check if first chef is currently better ranked than the second.
 *
 * The chef is better ranked if he has more points, or if he has equal 
 * points and his name is greater alphabetically
 * 
 * @param first First chef.
 * @param second Second chef.
 * @param firstBetter Will be "true" If first chef is better, "false" otherwise 
 * @return Result success or error code.
 *	Error codes: CHEF_NULL_ARGUMENT
 */
ChefResult chefIsBetterRanked(Chef first, Chef second, bool* firstBetter);

#endif // _CHEF_H