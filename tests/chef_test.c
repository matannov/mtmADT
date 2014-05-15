#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "test_utilities.h"
#include "../chef.h"
#include "../common.h"

#define ASSERT_CREATE_FAIL(a) \
	ASSERT_TEST(chefCreate(name,&result) == NULL && result == (a))

#define ASSERT_CREATE_SUCCESS() \
	ASSERT_TEST(chefCreate(name,&result) != NULL && result == CHEF_SUCCESS)

#define DEFINE_BASIC_NAME const char* basicName = "blabla";

#define DEFINE_BASIC_CHEF \
	DEFINE_BASIC_NAME; \
	Chef basicChef = chefCreate(basicName,NULL)

#define DEFINE_BASIC_DISH_PARAMS const char* basicDishName = "blabla"; \
		DishType const basicType = DISH_ENTREE; \
		Taste const basicTaste = {1, 2, 3} \

#define DEFINE_BASIC_DISH \
	DEFINE_BASIC_DISH_PARAMS; \
	Dish basicDish = dishCreate(basicDishName,basicType,basicTaste,NULL)

/***************** Tests *****************
 *****************************************/
static bool chefCreateTest() {
	DEFINE_BASIC_NAME;

	const char* name = basicName;
	ChefResult result;
	ASSERT_CREATE_SUCCESS();

	name = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890\
		~!@#$%^&*()_+[]{};':\"\\,./<>? ";
	ASSERT_CREATE_SUCCESS();
	name = NULL;
	ASSERT_CREATE_FAIL(CHEF_NULL_ARGUMENT);
	name = basicName;
	ASSERT_TEST(chefCreate(name,NULL) != NULL);
	return true;
}

static bool chefDestroyTest() {
	DEFINE_BASIC_CHEF;
	chefDestroy(basicChef);
	chefDestroy(NULL);
	return true;
}

static bool chefGetNameTest() {
	DEFINE_BASIC_CHEF;
	char* name;
	ASSERT_TEST(chefGetName(basicChef,&name) == CHEF_SUCCESS);
	ASSERT_TEST(STR_EQUALS(name,basicName));
	free(name);
	ASSERT_TEST(chefGetName(NULL,&name) == CHEF_NULL_ARGUMENT);
	ASSERT_TEST(chefGetName(basicChef,NULL) == CHEF_NULL_ARGUMENT);
	chefDestroy(basicChef);
	return true;
}

static bool chefAddDishTest() {
	DEFINE_BASIC_CHEF;
	DEFINE_BASIC_DISH;
	int priority = 0;
	ASSERT_TEST(chefAddDish(basicChef, basicDish, priority) == CHEF_SUCCESS);
	ASSERT_TEST(chefAddDish(basicChef, basicDish, priority) == CHEF_SUCCESS);
	priority = 234;
	ASSERT_TEST(chefAddDish(basicChef, basicDish, priority) == CHEF_SUCCESS);
	ASSERT_TEST(chefAddDish(NULL, basicDish, priority) == CHEF_NULL_ARGUMENT);
	ASSERT_TEST(chefAddDish(basicChef, NULL, priority) == CHEF_NULL_ARGUMENT);
	ASSERT_TEST(chefAddDish(basicChef, basicDish, -1) 
		== CHEF_BAD_PRIORITY);
	dishDestroy(basicDish);
	chefDestroy(basicChef);
	return true;
}

static bool chefGetTopDishNameTest() {
	DEFINE_BASIC_CHEF;
	char* name;
	ASSERT_TEST(chefGetTopDishName(basicChef, &name) == CHEF_HAS_NO_DISHES);
	
	DEFINE_BASIC_DISH;
	int priority = 0;
	ASSERT_TEST(chefAddDish(basicChef, basicDish, priority) == CHEF_SUCCESS);
	ASSERT_TEST(chefGetTopDishName(basicChef, &name) == CHEF_SUCCESS);
	ASSERT_TEST(STR_EQUALS(name, basicDishName));
	free(name);
	ASSERT_TEST(chefGetTopDishName(NULL, &name) == CHEF_NULL_ARGUMENT);
	ASSERT_TEST(chefGetTopDishName(basicChef, NULL) == CHEF_NULL_ARGUMENT);

	priority = 0;
	ASSERT_TEST(chefAddDish(basicChef, basicDish, priority) == CHEF_SUCCESS);
	priority = 1;
	ASSERT_TEST(chefAddDish(basicChef, basicDish, priority) == CHEF_SUCCESS);
	char const* specialName = "secretSauce";
	Dish specialDish = dishCreate(specialName,basicType,basicTaste,NULL);
	priority = 2;
	ASSERT_TEST(chefAddDish(basicChef, specialDish, priority) == 
		CHEF_SUCCESS);
	ASSERT_TEST(chefGetTopDishName(basicChef, &name) == CHEF_SUCCESS);
	ASSERT_TEST(STR_EQUALS(name, specialName));
	free(name);
	dishDestroy(specialDish);

	dishDestroy(basicDish);
	chefDestroy(basicChef);
	return true;
}

static bool chefGetPointsTest() {
	DEFINE_BASIC_CHEF;
	int points;
	ASSERT_TEST(chefGetPoints(basicChef, &points) == CHEF_SUCCESS);
	ASSERT_TEST(points == 0);
	ASSERT_TEST(chefGetPoints(NULL, &points) == CHEF_NULL_ARGUMENT);
	ASSERT_TEST(chefGetPoints(basicChef, NULL) == CHEF_NULL_ARGUMENT);
	chefDestroy(basicChef);
	return true;
}

static bool chefCompareNamesTest() {
	DEFINE_BASIC_CHEF;
	int result;
	ASSERT_TEST(chefCompareNames(basicChef, basicChef, &result) == 
		CHEF_SUCCESS);
	ASSERT_TEST(result == 0);

	Chef specialChef = chefCreate("CookieMonster",NULL);
	ASSERT_TEST(chefCompareNames(basicChef, specialChef, &result) == 
		CHEF_SUCCESS);
	ASSERT_TEST(result != 0);
	chefDestroy(specialChef);
	
	ASSERT_TEST(chefCompareNames(NULL, basicChef, &result) == 
		CHEF_NULL_ARGUMENT);
	ASSERT_TEST(chefCompareNames(basicChef, NULL, &result) == 
		CHEF_NULL_ARGUMENT);
	ASSERT_TEST(chefCompareNames(basicChef, basicChef, NULL) == 
		CHEF_NULL_ARGUMENT);
	chefDestroy(basicChef);
	return true;
}

static bool chefHasDishTest() {
	DEFINE_BASIC_CHEF;
	ASSERT_TEST(chefHasDish(basicChef) == false);
	DEFINE_BASIC_DISH;
	int priority = 0;
	ASSERT_TEST(chefAddDish(basicChef, basicDish, priority) == CHEF_SUCCESS);
	ASSERT_TEST(chefHasDish(basicChef) == true);
	dishDestroy(basicDish);
	ASSERT_TEST(chefHasDish(NULL) == false);
	chefDestroy(basicChef);
	return true;
}

static bool chefTakeTopDishTest() {
	DEFINE_BASIC_CHEF;
	Dish dish;
	ASSERT_TEST(chefTakeTopDish(basicChef, &dish) == CHEF_HAS_NO_DISHES);
	DEFINE_BASIC_DISH;
	int priority = 0;
	ASSERT_TEST(chefAddDish(basicChef, basicDish, priority) == CHEF_SUCCESS);
	ASSERT_TEST(chefTakeTopDish(basicChef, &dish) == CHEF_SUCCESS);
	ASSERT_TEST(chefHasDish(basicChef) == false);
	dishDestroy(dish);

	priority = 0;
	ASSERT_TEST(chefAddDish(basicChef, basicDish, priority) == CHEF_SUCCESS);
	char const* specialName = "special";
	Dish specialDish = dishCreate(specialName,basicType,basicTaste,NULL);
	priority = 1;
	ASSERT_TEST(chefAddDish(basicChef, specialDish, priority) == 
		CHEF_SUCCESS);
	ASSERT_TEST(chefTakeTopDish(basicChef, &dish) == CHEF_SUCCESS);
	ASSERT_TEST(chefHasDish(basicChef) == true);
	char* name;
	ASSERT_TEST(dishGetName(dish, &name) == DISH_SUCCESS);
	ASSERT_TEST(STR_EQUALS(name, specialName));
	dishDestroy(dish);
	free(name);

	ASSERT_TEST(chefTakeTopDish(NULL, &dish) == CHEF_NULL_ARGUMENT);
	ASSERT_TEST(chefTakeTopDish(basicChef, NULL) == CHEF_NULL_ARGUMENT);
	
	dishDestroy(basicDish);
	chefDestroy(basicChef);
	return true;
}

static bool chefGivePointTest() {
	DEFINE_BASIC_CHEF;
	ASSERT_TEST(chefGivePoint(basicChef) == CHEF_SUCCESS);
	int points;
	ASSERT_TEST(chefGetPoints(basicChef, &points) == CHEF_SUCCESS);
	ASSERT_TEST(points == 1);
	ASSERT_TEST(chefGivePoint(basicChef) == CHEF_SUCCESS);
	ASSERT_TEST(chefGetPoints(basicChef, &points) == CHEF_SUCCESS);
	ASSERT_TEST(points == 2);
		ASSERT_TEST(chefGivePoint(basicChef) == CHEF_SUCCESS);
	ASSERT_TEST(chefGetPoints(basicChef, &points) == CHEF_SUCCESS);
	ASSERT_TEST(points == 3);
	ASSERT_TEST(chefGivePoint(NULL) == CHEF_NULL_ARGUMENT);
	chefDestroy(basicChef);
	return true;
}

static bool chefIsBetterRankedTest() {
	DEFINE_BASIC_CHEF;
	bool firstBetter;
	ASSERT_TEST(chefIsBetterRanked(basicChef, basicChef, &firstBetter) ==
		CHEF_SUCCESS);
	ASSERT_TEST(firstBetter == false);

	Chef masterChef = chefCreate("Stefan",NULL);
	ASSERT_TEST(chefGivePoint(masterChef) == CHEF_SUCCESS);
	ASSERT_TEST(chefIsBetterRanked(masterChef, basicChef, &firstBetter) ==
		CHEF_SUCCESS);
	ASSERT_TEST(firstBetter == true);
	chefDestroy(masterChef);

	ASSERT_TEST(chefIsBetterRanked(NULL, basicChef, &firstBetter) ==
		CHEF_NULL_ARGUMENT);
	ASSERT_TEST(chefIsBetterRanked(basicChef, NULL, &firstBetter) ==
		CHEF_NULL_ARGUMENT);
	ASSERT_TEST(chefIsBetterRanked(basicChef, basicChef, NULL) ==
		CHEF_NULL_ARGUMENT);
	chefDestroy(basicChef);
	return true;
}

static bool chefCopyTest() {
	DEFINE_BASIC_CHEF;
	DEFINE_BASIC_DISH;
	int priority = 0;
	ASSERT_TEST(chefAddDish(basicChef, basicDish, priority) == 
		CHEF_SUCCESS);
	int const givePoints = 100;
	for(int i=0; i<givePoints; i++) {
		ASSERT_TEST(chefGivePoint(basicChef) == CHEF_SUCCESS);
	}
	Chef copy = chefCopy(basicChef);
	ASSERT_TEST(copy != NULL);
	char* name;
	chefGetName(copy,&name);
	ASSERT_TEST(STR_EQUALS(name,basicName));
	free(name);
	Dish dish;
	ASSERT_TEST(chefTakeTopDish(copy, &dish) == CHEF_SUCCESS);
	ASSERT_TEST(dishGetName(dish, &name) == DISH_SUCCESS);
	ASSERT_TEST(STR_EQUALS(name,basicDishName));
	free(name);
	dishDestroy(dish);
	ASSERT_TEST(chefHasDish(copy) == false);
	int points;
	ASSERT_TEST(chefGetPoints(copy, &points) == CHEF_SUCCESS);
	ASSERT_TEST(points == givePoints);
	chefDestroy(copy);

	ASSERT_TEST(chefCopy(NULL) == NULL);
	chefDestroy(basicChef);
	return true;
}

int main() {
	RUN_TEST(chefCreateTest);
	RUN_TEST(chefDestroyTest);
	RUN_TEST(chefGetNameTest);
	RUN_TEST(chefAddDishTest);
	RUN_TEST(chefGetTopDishNameTest);
	RUN_TEST(chefGetPointsTest);
	RUN_TEST(chefCompareNamesTest);
	RUN_TEST(chefIsBetterRankedTest);
	RUN_TEST(chefHasDishTest);
	RUN_TEST(chefTakeTopDishTest);
	RUN_TEST(chefGivePointTest);
	RUN_TEST(chefCopyTest);
	return 0;
}
