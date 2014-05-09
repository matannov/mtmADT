#include <stdlib.h>
#include <string.h>
#include "test_utilities.h"
#include "../chef.h"
#include "../common.h"
#include <stdbool.h>

#define ASSERT_CREATE_FAIL(a) \
	ASSERT_TEST(chefCreate(name,&result) == NULL && result == (a))

#define ASSERT_CREATE_SUCCESS() \
	ASSERT_TEST(chefCreate(name,&result) != NULL && result == CHEF_SUCCESS)

#define DEFINE_BASIC_NAME const char* basicName = "blabla";

#define DEFINE_BASIC_CHEF \
	DEFINE_BASIC_NAME; \
	Chef basicChef = chefCreate(basicName,NULL)

/***************** Tests *****************
 *****************************************/
static bool chefCreateTest() {
	DEFINE_BASIC_NAME;

	const char* name = basicName;
	ChefResult result;
	ASSERT_CREATE_SUCCESS();

	name = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890~!@#$%^&*()_+[]{};':\"\\,./<>? ";
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
	char buffer[20];
	ASSERT_TEST(chefGetName(basicChef,buffer) == CHEF_SUCCESS);
	ASSERT_TEST(STR_EQUALS(buffer,basicName));
	ASSERT_TEST(chefGetName(NULL,buffer) == CHEF_NULL_ARGUMENT);
	ASSERT_TEST(chefGetName(basicChef,NULL) == CHEF_NULL_ARGUMENT);
	chefDestroy(basicChef);
	return true;
}

//ChefResult chefAddDish(Chef chef, Dish dish, int priority);


//ChefResult chefGetTopDish(Chef chef, char* buffer);
//hmm how about really getting the dish?

//ChefResult chefGetPoints(Chef chef, int* points);
//add point?

static bool chefCopyTest() {
	DEFINE_BASIC_CHEF;
	Chef copy = chefCopy(basicChef);
	ASSERT_TEST(copy != NULL);
	char buffer[20];
	chefGetName(copy,buffer);
	ASSERT_TEST(STR_EQUALS(buffer,basicName));
	ASSERT_TEST(chefCopy(NULL) == NULL);
	chefDestroy(copy);
	chefDestroy(basicChef);
	return true;
}

//ChefResult chefIsBetterRanked(Chef first, Chef second, bool* firstBetter);

int main() {
	RUN_TEST(chefCreateTest);
	RUN_TEST(chefDestroyTest);
	RUN_TEST(chefGetNameTest);
	RUN_TEST(chefCopyTest);
	return 0;
}
