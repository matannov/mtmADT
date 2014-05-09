#include <stdbool.h>
#include "judge_preferences.h"

/* the common part of judge preferences
 * if one dish is unedible, preference does not depend on taste*/
#define CHECK_AND_JUDGE_UNEDIBLE(isUnedible1, isUnedible2) \
	if((isUnedible1) || (isUnedible2)) { \
		return judgeUnedible((isUnedible1), (isUnedible2)); \
	}

/* judge between dishes when atleast one is unedible */
static JudgePreference judgeUnedible(bool isUnedible1, bool isUnedible2) {
	if(!isUnedible1 && isUnedible2) {
		return JUDGE_PREFER_FIRST;
	}
	if(isUnedible1 && !isUnedible2) {
		return JUDGE_PREFER_SECOND;
	}
	return JUDGE_TIE;
}

/* prefer greater value
 * return: JUDGE_PREFER_FIRST if value1 is greater,
 * JUDGE_PREFER_SECOND if value2 is greater and JUDGE_TIE if equal */
static JudgePreference preferGreater(int value1, int value2) {
	if(value1 > value2) {
		return JUDGE_PREFER_FIRST;
	}
	if(value2 > value1) {
		return JUDGE_PREFER_SECOND;
	}
	return JUDGE_TIE;
}

/* prefer lesser value */
static JudgePreference preferLesser(int value1, int value2) {
	return preferGreater(value2, value1);
}

/* compare dish types */
static bool isBetterDishType(DishType type1, DishType type2) {
	return ((type1 == DISH_DESERT && type2 != DISH_DESERT) ||
		(type1 == DISH_ENTREE && type2 == DISH_APPETIZER));
}


JudgePreference judgeByPreference1(Dish dish1, Dish dish2, 
	bool isUnedible1, bool isUnedible2) {

	CHECK_AND_JUDGE_UNEDIBLE(isUnedible1, isUnedible2)
	Taste taste1, taste2;
	dishGetTaste(dish1, &taste1);
	dishGetTaste(dish2, &taste2);
	return preferGreater(taste1.sweetness, taste2.sweetness);
}

JudgePreference judgeByPreference2(Dish dish1, Dish dish2, 
	bool isUnedible1, bool isUnedible2) {
	
	CHECK_AND_JUDGE_UNEDIBLE(isUnedible1, isUnedible2)
	Taste taste1, taste2;
	dishGetTaste(dish1, &taste1);
	dishGetTaste(dish2, &taste2);
	return preferGreater(taste1.saltiness, taste2.saltiness);
}

JudgePreference judgeByPreference3(Dish dish1, Dish dish2, 
	bool isUnedible1, bool isUnedible2) {
	
	CHECK_AND_JUDGE_UNEDIBLE(isUnedible1, isUnedible2)
	DishType type1, type2;
	dishGetType(dish1, &type1);
	dishGetType(dish2, &type2);
	if(isBetterDishType(type1, type2)) {
		return JUDGE_PREFER_FIRST;
	}
	if(isBetterDishType(type2, type1)) {
		return JUDGE_PREFER_SECOND;
	}
	Taste taste1, taste2;
	dishGetTaste(dish1, &taste1);
	dishGetTaste(dish2, &taste2);
	return preferLesser(taste1.sourness, taste2.sourness);
}
