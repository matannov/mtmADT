#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "common.h"
#include "chef.h"
#include "set.h"
#include "judge.h"

struct judge {
	char* nickname;
	Set hatedChefs;
	int badTastings;
	JudgeByPreference judgeByPreference;
};

/* check if judge should quit */
static bool shouldQuit(Judge judge) {
	return (judge->badTastings > JUDGE_BAD_TASTING_LIMIT);
}

/* check if dish is unedible 
 * (here because could be changed to depend on the judge)*/
static bool isUnedibleDish(Dish dish) {
	int const unedibleSweetness = 10;
	int const unedibleSourness = 8;
	int const unedibleSaltiness = 6;
	Taste taste;
	dishGetTaste(dish, &taste);
	return (taste.sweetness >= unedibleSweetness || 
		taste.sourness >= unedibleSourness || 
		taste.saltiness >= unedibleSaltiness);
} 

void * getInedibleFunction() {
	return &isUnedibleDish;
}

/* check if a chef is hated based on past and current unedible dish.
 * also update badTastings count */
static bool CheckAndUpdateHated(Judge judge, bool isUnedible, char* chefName) {
	if(isUnedible) {
		setAdd(judge->hatedChefs, chefName);
		judge->badTastings++;
		return true;
	}
	return setIsIn(judge->hatedChefs, chefName);
}

/* check if chef name is prior lexicographically */
static bool isNamePrior(char* chefName1, char* chefName2) {
	return (strcmp(chefName1, chefName2) < 0);
}

/* wrap cloneString for use in set */
static SetElement copyName(SetElement name) {
	return cloneString(name);
}

/* wrap strcmp for use in set */
static int compareNames(SetElement first, SetElement second) {
	return strcmp(first, second);
}

Judge judgeCreate(char const* nickname, JudgeByPreference judgeByPreference, 
	JudgeResult* result) {
	
	if(nickname == NULL || judgeByPreference == NULL) {
		ASSIGN_AND_RETURN(result, JUDGE_NULL_ARGUMENT, NULL)
	}
	Judge judge = malloc(sizeof(*judge));
	if(judge == NULL) {
		ASSIGN_AND_RETURN(result, JUDGE_OUT_OF_MEMORY, NULL)
	}
	judge->nickname = cloneString(nickname);
	judge->hatedChefs = setCreate(&copyName, &free, &compareNames);
	if(judge->nickname == NULL || judge->hatedChefs == NULL) {
		judgeDestroy(judge);		
		ASSIGN_AND_RETURN(result, JUDGE_OUT_OF_MEMORY, NULL)
	}
	
	judge->badTastings = 0;
	judge->judgeByPreference = judgeByPreference;
	ASSIGN_AND_RETURN(result, JUDGE_SUCCESS, judge)
}

void judgeDestroy(Judge judge) {
	if(judge == NULL) {
		return;
	}
	setDestroy(judge->hatedChefs);
	free(judge->nickname);
	free(judge);
}

Judge judgeCopy(Judge source) {
	if(source == NULL) {
		return NULL;
	}
	Judge copy = judgeCreate(source->nickname, source->judgeByPreference,
		NULL);
	if(copy == NULL) {
		return NULL;
	}
	setDestroy(copy->hatedChefs);
	copy->hatedChefs = setCopy(source->hatedChefs);
	if(copy->hatedChefs == NULL) {
		judgeDestroy(copy);
		return NULL;
	}
	copy->badTastings = source->badTastings;
	return copy;
}

JudgeResult judgeGetNickname(Judge judge, char* buffer) {
	if(buffer == NULL || judge == NULL) {
		return JUDGE_NULL_ARGUMENT;
	}
	strcpy(buffer, judge->nickname);
	return JUDGE_SUCCESS;
}

JudgeResult judgeJudgeDishes(Judge judge, Dish dish1, Dish dish2, 
	char* chefName1, char* chefName2, bool* dish1Wins, bool* judgeQuits) {

	if(judge == NULL || dish1 == NULL || dish2 == NULL || chefName1 == NULL
		|| chefName2 == NULL || dish1Wins == NULL || judgeQuits == NULL) {
		return JUDGE_NULL_ARGUMENT;
	}
	if(shouldQuit(judge)) {
		return JUDGE_ALREADY_QUITTED;
	}
	bool isUnedible1 = isUnedibleDish(dish1);
	bool isUnedible2 = isUnedibleDish(dish2);
	JudgePreference preference = judge->judgeByPreference(dish1, dish2, 
			isUnedible1, isUnedible2);
	if(!IN_RANGE(preference, JUDGE_PREFERENCE_MIN, JUDGE_PREFERENCE_MAX)) {
		return JUDGE_BAD_PREFERENCE_RESULT;
	}

	bool isHated1 = CheckAndUpdateHated(judge, isUnedible1, chefName1);
	bool isHated2 = CheckAndUpdateHated(judge, isUnedible2, chefName2);
	*dish1Wins = ((!isHated1 && isHated2) || (isHated1 == isHated2 && 
		(preference == JUDGE_PREFER_FIRST || (preference == JUDGE_TIE && 
		isNamePrior(chefName1, chefName2)))));
	*judgeQuits = shouldQuit(judge);
	return JUDGE_SUCCESS;
}

char * judgeGetName(Judge judge) {
	if (judge == NULL) {
		return NULL;
	}
	return judge->nickname;
}