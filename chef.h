#ifndef _CHEF_H
#define _CHEF_H

#include "dish.h"
#include <stdbool.h>

#define CHEF_NAME_LENGTH_MAX 63
#define DISH_PRIORITY_MIN 0

typedef enum {
	CHEF_SUCCESS,
	CHEF_OUT_OF_MEMORY,
	CHEF_NULL_ARGUMENT,
	CHEF_BAD_PARAM,
	CHEF_BAD_PRIORITY,
	CHEF_HAS_NO_DISHES
} ChefResult;

typedef struct chef* Chef;

/*
 * Create a new chef with given name.
 *
 * In case name is invalid or if run out of memory, 
 * an error code is set in "result".
 * If "result" is null, no error code is returned.
 *
 * @param name Name for new chef. The string is copied.
 * @param result Result success or error code.
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
 * @return Chef copy, NULL in case of an error.
 */
Chef chefCopy(Chef source);

/*
 * Get length of chef's name.
 *
 * @param chef chef to get name's length of
 * @param nameLength the length of the chef's name will be written here
 * @return Result success or error code
 *
*/

ChefResult chefGetNameLength(Chef chef, int* nameLength);

/*
 * Get chef name.
 *
 * @param chef Chef to get name of
 * @param buffer Name will be written here. Make sure there is enough space.
 * @return Result success or error code.
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
 */
ChefResult chefAddDish(Chef chef, Dish dish, int priority);

/*
 * Get the name of dish with highest priority. If some dishes have the 
 * same priority, the dish that was added first is returned.
 *
 * @param chef Chef to get the name of top dish from.
 * @param buffer Name will be written here. Make sure there is enough space.
 * @return Result success or error code.
 */
ChefResult chefGetTopDish(Chef chef, char* buffer);

/*
 * Get the number of points the chef has.
 *
 * @param chef Chef to get the points from.
 * @param points Points will be written here.
 * @return Result success or error code.
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
 */
ChefResult chefIsBetterRanked(Chef first, Chef second, bool* firstBetter);

/********************************
	isSameChef
	takes two Chefs and a pointer to a bool
	returns a chefResult,
	and writes true to the bool if they have the same name,
	and false otherwise
********************************/
/* why do we need this?
ChefResult isSameChef(Chef first, Chef second, bool* areIdentical);
*/

#endif // _CHEF_H