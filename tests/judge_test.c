#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "test_utilities.h"
#include "../judge.h"
#include "../common.h"

#define ASSERT_CREATE_FAIL(a) \
	ASSERT_TEST(judgeCreate(name,preference,&result) == NULL && \
	result == (a))

#define ASSERT_CREATE_SUCCESS() \
	ASSERT_TEST(judgeCreate(name,preference,&result) != NULL && \
	result == JUDGE_SUCCESS)

#define DEFINE_BASIC_PARAMS \
	const char* basicName = "blabla";

#define DEFINE_BASIC_JUDGE \
	DEFINE_BASIC_PARAMS; \
	Judge basicJudge = judgeCreate(basicName,basicJudgeByPreference,NULL)

#define DEFINE_BASIC_DISH_PARAMS const char* basicDishName = "blabla"; \
		DishType const basicType = DISH_ENTREE; \
		Taste const basicTaste = {1, 2, 3} \

#define DEFINE_BASIC_DISH \
	DEFINE_BASIC_DISH_PARAMS; \
	Dish basicDish = dishCreate(basicDishName,basicType,basicTaste,NULL)

static JudgePreference basicJudgeByPreference(Dish dish1, Dish dish2, 
	bool isUnedible1, bool isUnedible2) {
	return JUDGE_PREFER_FIRST;
}

/***************** Tests *****************
 *****************************************/
static bool judgeCreateTest() {
	DEFINE_BASIC_PARAMS;
	const char* name = basicName;
	JudgeByPreference preference = basicJudgeByPreference;
	JudgeResult result;
	ASSERT_CREATE_SUCCESS();
	name = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890\
		~!@#$%^&*()_+[]{};':\"\\,./<>? ";
	ASSERT_CREATE_SUCCESS();
	name = NULL;
	ASSERT_CREATE_FAIL(JUDGE_NULL_ARGUMENT);
	name = basicName;
	preference = NULL;
	ASSERT_CREATE_FAIL(JUDGE_NULL_ARGUMENT);
	preference = basicJudgeByPreference;
	ASSERT_TEST(judgeCreate(name,preference,NULL) != NULL);
	return true;
}

static bool judgeDestroyTest() {
	DEFINE_BASIC_JUDGE;
	judgeDestroy(basicJudge);
	judgeDestroy(NULL);
	return true;
}

static bool judgeGetNicknameTest() {
	DEFINE_BASIC_JUDGE;
	char* name;
	ASSERT_TEST(judgeGetNickname(basicJudge,&name) == JUDGE_SUCCESS);
	ASSERT_TEST(STR_EQUALS(name,basicName));
	free(name);
	ASSERT_TEST(judgeGetNickname(NULL,&name) == JUDGE_NULL_ARGUMENT);
	ASSERT_TEST(judgeGetNickname(basicJudge,NULL) == JUDGE_NULL_ARGUMENT);
	judgeDestroy(basicJudge);
	return true;
}

static bool judgeCopyTest() {
	DEFINE_BASIC_JUDGE;
	Judge copy = judgeCopy(basicJudge);
	ASSERT_TEST(copy != NULL);
	char* name;
	ASSERT_TEST(judgeGetNickname(copy,&name) == JUDGE_SUCCESS);
	ASSERT_TEST(STR_EQUALS(name,basicName));
	free(name);

	ASSERT_TEST(judgeCopy(NULL) == NULL);
	judgeDestroy(basicJudge);
	return true;
}

static bool judgeJudgeDishesTest() {
	DEFINE_BASIC_JUDGE;
	DEFINE_BASIC_DISH;
	char* chefName1 = "someChef";
	char* chefName2 = "otherChef";
	bool dish1Wins, judgeQuits;
	ASSERT_TEST(judgeJudgeDishes(basicJudge, basicDish, basicDish, chefName1, 
		chefName2, &dish1Wins, &judgeQuits) == JUDGE_SUCCESS);
	ASSERT_TEST(dish1Wins == true);
	ASSERT_TEST(judgeQuits == false);
	
	ASSERT_TEST(judgeJudgeDishes(NULL, basicDish, basicDish, chefName1, 
		chefName2, &dish1Wins, &judgeQuits) == JUDGE_NULL_ARGUMENT);
	ASSERT_TEST(judgeJudgeDishes(basicJudge, NULL, basicDish, chefName1, 
		chefName2, &dish1Wins, &judgeQuits) == JUDGE_NULL_ARGUMENT);
	ASSERT_TEST(judgeJudgeDishes(basicJudge, basicDish, NULL, chefName1, 
		chefName2, &dish1Wins, &judgeQuits) == JUDGE_NULL_ARGUMENT);
	ASSERT_TEST(judgeJudgeDishes(basicJudge, basicDish, basicDish, NULL, 
		chefName2, &dish1Wins, &judgeQuits) == JUDGE_NULL_ARGUMENT);
	ASSERT_TEST(judgeJudgeDishes(basicJudge, basicDish, basicDish, chefName1, 
		NULL, &dish1Wins, &judgeQuits) == JUDGE_NULL_ARGUMENT);
	ASSERT_TEST(judgeJudgeDishes(basicJudge, basicDish, basicDish, chefName1, 
		chefName2, NULL, &judgeQuits) == JUDGE_NULL_ARGUMENT);
	ASSERT_TEST(judgeJudgeDishes(basicJudge, basicDish, basicDish, chefName1, 
		chefName2, &dish1Wins, NULL) == JUDGE_NULL_ARGUMENT);
	dishDestroy(basicDish);
	judgeDestroy(basicJudge);
	return true;
}

int main() {
	RUN_TEST(judgeCreateTest);
	RUN_TEST(judgeDestroyTest);
	RUN_TEST(judgeGetNicknameTest);
	RUN_TEST(judgeJudgeDishesTest);
	RUN_TEST(judgeCopyTest);
	return 0;
}
