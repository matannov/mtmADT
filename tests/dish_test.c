#include <stdlib.h>
#include <string.h>
#include "test_utilities.h"
#include "../common.h"
#include "../dish.h"

#define ASSERT_CREATE_FAIL(a) \
	ASSERT_TEST(dishCreate(name,type,taste,&result) == NULL && result == (a))

#define ASSERT_CREATE_SUCCESS() \
	ASSERT_TEST(dishCreate(name,type,taste,&result) != NULL \
		&& result == DISH_SUCCESS)

#define DEFINE_BASIC_PARAMS const char* basicName = "blabla"; \
		DishType const basicType = DISH_ENTREE; \
		Taste const basicTaste = {1, 2, 3} \

#define DEFINE_BASIC_DISH \
	DEFINE_BASIC_PARAMS; \
	Dish basicDish = dishCreate(basicName,basicType,basicTaste,NULL)

/***************** Tests *****************
 *****************************************/
static bool dishCreateTest() {
	DEFINE_BASIC_PARAMS;

	const char* name = basicName;
	DishType type = basicType;
	Taste taste = basicTaste;
	DishResult result;
	ASSERT_CREATE_SUCCESS();

	name = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890\
		~!@#$%^&*()_+[]{};':\"\\,./<>? ";
	ASSERT_CREATE_SUCCESS();
	name = NULL;
	ASSERT_CREATE_FAIL(DISH_NULL_ARGUMENT);
	name = basicName;

	type = DISH_TYPE_MAX;
	ASSERT_CREATE_SUCCESS();
	type = DISH_TYPE_MIN-1;
	ASSERT_CREATE_FAIL(DISH_BAD_PARAM);
	type = DISH_TYPE_MAX+1;
	ASSERT_CREATE_FAIL(DISH_BAD_PARAM);
	type = basicType;
	
	taste = (Taste){DISH_TASTE_PARAM_MIN, DISH_TASTE_PARAM_MIN, 
		DISH_TASTE_PARAM_MIN};
	ASSERT_CREATE_SUCCESS();
	taste = (Taste){DISH_TASTE_PARAM_MAX, DISH_TASTE_PARAM_MAX, 
		DISH_TASTE_PARAM_MAX};
	ASSERT_CREATE_SUCCESS();
	taste.saltiness = DISH_TASTE_PARAM_MIN-1;
	ASSERT_CREATE_FAIL(DISH_BAD_PARAM);
	taste.saltiness = DISH_TASTE_PARAM_MAX+1;
	ASSERT_CREATE_FAIL(DISH_BAD_PARAM);
	taste = basicTaste;

	ASSERT_TEST(dishCreate(name,type,taste,NULL) != NULL);
	return true;
}

static bool dishDestroyTest() {
	DEFINE_BASIC_DISH;
	dishDestroy(basicDish);
	dishDestroy(NULL);
	return true;
}

static bool dishGetNameTest() {
	DEFINE_BASIC_DISH;
	char* name;
	ASSERT_TEST(dishGetName(basicDish, &name) == DISH_SUCCESS);
	ASSERT_TEST(STR_EQUALS(name, basicName));
	free(name);
	ASSERT_TEST(dishGetName(NULL, &name) == DISH_NULL_ARGUMENT);
	ASSERT_TEST(dishGetName(basicDish, NULL) == DISH_NULL_ARGUMENT);
	dishDestroy(basicDish);
	return true;
}

static bool dishGetTasteTest() {
	DEFINE_BASIC_DISH;
	Taste taste;
	ASSERT_TEST(dishGetTaste(basicDish, &taste) == DISH_SUCCESS);
	ASSERT_TEST(memcmp(&taste,&basicTaste,sizeof(taste)) == 0);
	ASSERT_TEST(dishGetTaste(NULL, &taste) == DISH_NULL_ARGUMENT);
	ASSERT_TEST(dishGetTaste(basicDish, NULL) == DISH_NULL_ARGUMENT);
	dishDestroy(basicDish);
	return true;
}

static bool dishGetTypeTest() {
	DEFINE_BASIC_DISH;
	DishType type;
	ASSERT_TEST(dishGetType(basicDish, &type) == DISH_SUCCESS);
	ASSERT_TEST(type == basicType);
	ASSERT_TEST(dishGetType(NULL, &type) == DISH_NULL_ARGUMENT);
	ASSERT_TEST(dishGetType(basicDish, NULL) == DISH_NULL_ARGUMENT);
	dishDestroy(basicDish);
	return true;
}

static bool dishCopyTest() {
	DEFINE_BASIC_DISH;
	Dish copy = dishCopy(basicDish);
	ASSERT_TEST(copy != NULL);
	char* name;
	Taste taste;
	DishType type;
	dishGetName(copy,&name);
	dishGetTaste(copy,&taste);
	dishGetType(copy,&type);
	ASSERT_TEST(STR_EQUALS(name,basicName) &&
		memcmp(&taste,&basicTaste,sizeof(taste)) == 0 &&
		type == basicType);
	free(name);
	ASSERT_TEST(dishCopy(NULL) == NULL);
	dishDestroy(copy);
	dishDestroy(basicDish);
	return true;
}

int main() {
	RUN_TEST(dishCreateTest);
	RUN_TEST(dishDestroyTest);
	RUN_TEST(dishGetNameTest);
	RUN_TEST(dishGetTasteTest);
	RUN_TEST(dishGetTypeTest);
	RUN_TEST(dishCopyTest);
	return 0;
}