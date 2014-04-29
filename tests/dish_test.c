#include <stdlib.h>
#include <string.h>
#include "../dish.h"
#include "test_utilities.h"

#define ASSERT_BAD_PARAM(val) ASSERT_TEST(val == DISH_BAD_PARAM)
#define ASSERT_SUCCESS(val) ASSERT_TEST(val == DISH_SUCCESS)

bool dishCreateTest();

bool dishCreateTest() {
	dishResult * results = (dishResult*)malloc(sizeof(int)*5);
	char word[10];
	strcpy(word,"wordSoup");
	dishCreate(word,entree,-1,2,4,results);
	dishCreate(word,desert,3,20,4,results+1);
	dishCreate(word,appetizer,2,8,11,results+2);
	dishCreate(word,5,3,2,4,results+3);
	dishCreate(word,desert,3,2,4,results+4);
	ASSERT_BAD_PARAM(results[0]);
	ASSERT_BAD_PARAM(results[1]);
	ASSERT_BAD_PARAM(results[2]);
	ASSERT_BAD_PARAM(results[3]);
	ASSERT_SUCCESS(results[4]);
	free(results);
	return true;
}

int main() {
	RUN_TEST(dishCreateTest);
	return 0;
}