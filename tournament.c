#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "set.h"
#include "list.h"
#include "common.h"
#include "tournament.h"

struct Tournament_t {
	Set chefs;
	List judges;
};

/* wrap chefCopy for use in set */
static SetElement copyChef(SetElement chef) {
	return chefCopy(chef);
}

/* wrap chefDestroy for use in set */
static void destroyChef(SetElement chef) {
	chefDestroy(chef);
}

/* wrap chefCompareNames for use in set */
static int compareChefs(SetElement first, SetElement second) {
	int result;
	chefCompareNames(first, second, &result);
	return result;
}

/* wrap judgeCopy for use in list */
static ListElement copyJudge(ListElement judge) {
	return judgeCopy(judge);
}

/* wrap judgeDestroy for use in list */
static void destroyJudge(ListElement judge) {
	judgeDestroy(judge);
}

/* macro to call findChef and also return in case of error */
#define FIND_CHEF_RETURN_IF_ERROR(tournament, result, chefName, chef) \
	(result) = findChef((tournament), (chefName), (chef)); \
	if((result) != TOURNAMENT_SUCCESS) { \
		return (result); \
	}

/* find a chef in the set. 
 * error codes: TOURNAMENT_NO_SUCH_CHEF, TOURNAMENT_OUT_OF_MEMORY */
static TournamentResult findChef(Tournament tournament, char const* name, 
	Chef* matchingChef) {

	ChefResult getResult;
	char* currentName;
	bool foundName;
	SET_FOREACH(Chef, chef, tournament->chefs) {
		getResult = chefGetName(chef, &currentName);
		if(getResult == CHEF_OUT_OF_MEMORY) {
			return TOURNAMENT_OUT_OF_MEMORY;
		}
		assert(getResult == CHEF_SUCCESS);
		foundName = STR_EQUALS(currentName, name);
		free(currentName);
		if(foundName){
			*matchingChef = chef;
			return TOURNAMENT_SUCCESS;
		}
	}
	return TOURNAMENT_NO_SUCH_CHEF;
}

Tournament tournamentCreate(TournamentResult* result) {
	Tournament tournament = malloc(sizeof(*tournament));
	if(tournament == NULL) {
		ASSIGN_AND_RETURN(result, TOURNAMENT_OUT_OF_MEMORY, NULL)
	}
	tournament->chefs = setCreate(&copyChef, &destroyChef, &compareChefs);
	tournament->judges = listCreate(&copyJudge, &destroyJudge);
	if(tournament->chefs == NULL || tournament->judges == NULL) {
		tournamentDestroy(tournament);
		ASSIGN_AND_RETURN(result, TOURNAMENT_OUT_OF_MEMORY, NULL)
	}
	ASSIGN_AND_RETURN(result, TOURNAMENT_SUCCESS, tournament)
}

void tournamentDestroy(Tournament tournament) {
	if(tournament == NULL) {
		return;
	}
	listDestroy(tournament->judges);
	setDestroy(tournament->chefs);
	free(tournament);
}

TournamentResult tournamentAddChef(Tournament tournament, char const* name) {
	if(tournament == NULL || name == NULL) {
		return TOURNAMENT_NULL_ARGUMENT;
	}
	ChefResult chefResult;
	Chef chef = chefCreate(name, &chefResult);
	if(chefResult == CHEF_OUT_OF_MEMORY) {
		return TOURNAMENT_OUT_OF_MEMORY;
	}
	assert(chefResult == CHEF_SUCCESS);
	SetResult result = setAdd(tournament->chefs, chef);
	chefDestroy(chef);
	switch(result) {
	case SET_ITEM_ALREADY_EXISTS:
		return TOURNAMENT_CHEF_ALREADY_EXISTS;
	case SET_OUT_OF_MEMORY:
		return TOURNAMENT_OUT_OF_MEMORY;
	default:
		assert(result == SET_SUCCESS);
	}
	return TOURNAMENT_SUCCESS;
}

TournamentResult tournamentLeadingChef(Tournament tournament, Chef* leader) {
	if(tournament == NULL || leader == NULL) {
		return TOURNAMENT_NULL_ARGUMENT;
	}
	if(setGetSize(tournament->chefs) == 0) {
		return TOURNAMENT_HAS_NO_CHEFS;
	}
	bool isBetter = false;
	Chef best = (Chef)setGetFirst(tournament->chefs);
	SET_FOREACH(Chef, chef, tournament->chefs) {
		chefIsBetterRanked(chef, best, &isBetter);
		if(isBetter) {
			best = chef;
		}
	}
	*leader = best;
	return TOURNAMENT_SUCCESS;
}

TournamentResult tournamentAddJudge(Tournament tournament, char const* nickname, 
	JudgeByPreference judgeByPreference) {

	if(nickname == NULL || tournament == NULL || judgeByPreference == NULL) {
		return TOURNAMENT_NULL_ARGUMENT;
	}
	JudgeResult result;
	Judge judge = judgeCreate(nickname, judgeByPreference, &result);
	if(result == JUDGE_OUT_OF_MEMORY) {
		return TOURNAMENT_OUT_OF_MEMORY;
	}
	assert(result == JUDGE_SUCCESS);
	ListResult listResult = listInsertLast(tournament->judges, judge);
	judgeDestroy(judge);
	if(listResult == LIST_OUT_OF_MEMORY) {
		return TOURNAMENT_OUT_OF_MEMORY;
	}
	assert(listResult == LIST_SUCCESS);
	return TOURNAMENT_SUCCESS;
}

TournamentResult tournamentGetJudges(Tournament tournament, char*** judges, 
	int* numberOfJudges) {

	if(tournament == NULL || judges == NULL || numberOfJudges == NULL) {
		return TOURNAMENT_NULL_ARGUMENT;
	}
	*numberOfJudges = listGetSize(tournament->judges);
	if(*numberOfJudges == 0) {
		return TOURNAMENT_HAS_NO_JUDGES;
	}
	char** judgesArray = malloc(sizeof(*judgesArray)*(*numberOfJudges));
	if(judgesArray == NULL) {
		return TOURNAMENT_OUT_OF_MEMORY;
	}
	JudgeResult getResult;
	char** currentName = judgesArray;
	LIST_FOREACH(Judge, judge, tournament->judges) {
		getResult = judgeGetNickname(judge, currentName);
		switch(getResult) {
		case JUDGE_OUT_OF_MEMORY:
			freeArray((void**)judgesArray, currentName-judgesArray);
			return TOURNAMENT_OUT_OF_MEMORY;
		default:
			assert(getResult == JUDGE_SUCCESS);
		}
		currentName++;
	}
	*judges = judgesArray;
	return TOURNAMENT_SUCCESS;
}

TournamentResult tournamentAddDishToChef(Tournament tournament, 
	char const* chefName, char const* dishName, DishType type, Taste taste, 
	int priority) {
	
	if(tournament == NULL || chefName == NULL || dishName == NULL) {
		return TOURNAMENT_NULL_ARGUMENT;
	}
	Chef target;
	TournamentResult tournamentResult = findChef(tournament, chefName, 
		&target);
	if(tournamentResult != TOURNAMENT_SUCCESS) {
		return tournamentResult;
	}

	DishResult dishResult;
	Dish dish = dishCreate(dishName, type, taste, &dishResult);
	switch(dishResult) {
	case DISH_BAD_PARAM:
		return TOURNAMENT_BAD_PARAM;
	case DISH_OUT_OF_MEMORY:
		return TOURNAMENT_OUT_OF_MEMORY;
	default:
		assert(dishResult == DISH_SUCCESS);
	}

	ChefResult chefResult = chefAddDish(target, dish, priority);
	dishDestroy(dish);
	switch(chefResult) {
	case CHEF_BAD_PRIORITY:
		return TOURNAMENT_BAD_PARAM;
	case CHEF_OUT_OF_MEMORY:
		return TOURNAMENT_OUT_OF_MEMORY;
	default:
		assert(chefResult == CHEF_SUCCESS);
	}
	return TOURNAMENT_SUCCESS;
}

TournamentResult tournamentGetTopDish(Tournament tournament, 
	char const* chefName, char** dishName) {

	if(chefName == NULL || tournament == NULL || dishName == NULL) {
		return TOURNAMENT_NULL_ARGUMENT;
	}
	Chef chef;
	TournamentResult result;
	FIND_CHEF_RETURN_IF_ERROR(tournament, result, chefName, &chef)
	ChefResult chefResult = chefGetTopDishName(chef, dishName);
	switch(chefResult) {
	case CHEF_HAS_NO_DISHES:
		return TOURNAMENT_CHEF_HAS_NO_DISHES;
	case CHEF_OUT_OF_MEMORY:
		return TOURNAMENT_OUT_OF_MEMORY;
	default:
		assert(chefResult == CHEF_SUCCESS);
	}
	return TOURNAMENT_SUCCESS;
}

/* initialization for compete put into seperate function
 * error codes: TOURNAMENT_NULL_ARGUMENT, TOURNAMENT_SAME_CHEF,
 * TOURNAMENT_CHEF_HAS_NO_DISHES, TOURNAMENT_OUT_OF_MEMORY */
TournamentResult competeInitialize(Tournament tournament, 
	char const* chefName1, char const* chefName2, char*** resigningJudges,
	int* resigningCount, bool* firstWins, bool* secondWins, 
	int* initialJudges, Chef* chef1, Chef* chef2) {

	if(tournament == NULL || chefName1 == NULL || chefName2 == NULL ||
		resigningJudges == NULL || resigningCount == NULL ||
		firstWins == NULL || secondWins == NULL) {
		return TOURNAMENT_NULL_ARGUMENT;
	}
	TournamentResult result;
	FIND_CHEF_RETURN_IF_ERROR(tournament, result, chefName1, chef1)
	FIND_CHEF_RETURN_IF_ERROR(tournament, result, chefName2, chef2)
	if(STR_EQUALS(chefName1, chefName2)) {
		return TOURNAMENT_SAME_CHEF;
	}
	if(!chefHasDish(*chef1) || !chefHasDish(*chef2)) {
		return TOURNAMENT_CHEF_HAS_NO_DISHES;
	}
	*resigningCount = 0;
	*initialJudges = listGetSize(tournament->judges);	
	*resigningJudges = malloc(sizeof(**resigningJudges)*(*initialJudges));
	if(*resigningJudges == NULL) {
		return TOURNAMENT_OUT_OF_MEMORY;
	}
	return TOURNAMENT_SUCCESS;
}

/* compete only between two dishes. 
 * resigningJudges is assumed to be already allocated. more names are added
 * at "resigningCount" index.
 * resigningCount is assumed to be initialized. it is incremented.
 * chefPoints1 and chefPoints2 are also incremented.
 * error codes: TOURNAMENT_BAD_PREFERENCE_RESULT, TOURNAMENT_OUT_OF_MEMORY */
static TournamentResult competeTwoDishes(Tournament tournament, Dish dish1, 
	Dish dish2, char* chefName1, char* chefName2, 
	char*** resigningJudges, int* resigningCount, int* chefPoints1, 
	int* chefPoints2) {

	Judge currentJudge = listGetFirst(tournament->judges);
	int remainingJudges = listGetSize(tournament->judges);
	int dishPoints1 = 0, dishPoints2 = 0;
	while((DISTANCE(dishPoints1, dishPoints2) <= remainingJudges) && (remainingJudges > 0)) {
		bool firstWins, resignation;
		JudgeResult judgeResult = judgeJudgeDishes(currentJudge, dish1, dish2, 
			chefName1, chefName2, &firstWins, &resignation);
		if(judgeResult == JUDGE_BAD_PREFERENCE_RESULT) {
			return TOURNAMENT_BAD_PREFERENCE_RESULT;
		}
		assert(judgeResult == JUDGE_SUCCESS);
		if(firstWins) {
			dishPoints1++;
		} else {
			dishPoints2++;
		}
		if(resignation) {
			judgeResult = judgeGetNickname(currentJudge,
				((*resigningJudges) + *resigningCount));
			if(judgeResult == JUDGE_OUT_OF_MEMORY) {
				return TOURNAMENT_OUT_OF_MEMORY;
			}
			(*resigningCount)++;
			listRemoveCurrent(tournament->judges);
			currentJudge = listGetFirst(tournament->judges);
			for (int j = 0;
				j < listGetSize(tournament->judges) - remainingJudges - 1; j++) {
				currentJudge = listGetNext(tournament->judges);
			}
		}
		else {
			currentJudge = listGetNext(tournament->judges);
		}
		remainingJudges--;
	}
	if(dishPoints1 > dishPoints2) {
		(*chefPoints1)++;
	} else if(dishPoints2 > dishPoints1) {
		(*chefPoints2)++;
	}
	return TOURNAMENT_SUCCESS;
}

TournamentResult tournamentCompete(Tournament tournament, 
	char* chefName1, char* chefName2, char*** resigningJudges,
	int* resigningCount, bool* firstChefWins, bool* secondChefWins) {
	
	TournamentResult result;
	int initialJudges;
	Chef chef1, chef2;	
	result = competeInitialize(tournament, chefName1, chefName2, 
		resigningJudges, resigningCount, firstChefWins, secondChefWins,
		&initialJudges, &chef1, &chef2);
	if(result != TOURNAMENT_SUCCESS) {
		return result;
	}
	int chefPoints1 = 0, chefPoints2 = 0;
	do {
		if(*resigningCount == initialJudges) {
			return TOURNAMENT_HAS_NO_JUDGES;
		}
		Dish dish1 = NULL, dish2 = NULL;
		chefTakeTopDish(chef1, &dish1);
		chefTakeTopDish(chef2, &dish2);
		if(dish1 == NULL || dish2 == NULL) {
			dishDestroy(dish1);
			dishDestroy(dish2);
			freeArray((void**)*resigningJudges, *resigningCount);
			return TOURNAMENT_OUT_OF_MEMORY;
		}
		result = competeTwoDishes(tournament, dish1, dish2, chefName1, 
			chefName2, resigningJudges, resigningCount,
			&chefPoints1, &chefPoints2);
		dishDestroy(dish1);
		dishDestroy(dish2);
		if(result != TOURNAMENT_SUCCESS) {
			return result;
		}	
	} while(chefHasDish(chef1) && chefHasDish(chef2));

	*firstChefWins = chefPoints1 > chefPoints2;
	*secondChefWins = chefPoints2 > chefPoints1;
	if(*firstChefWins) {
		chefGivePoint(chef1);
	} else if(*secondChefWins) {
		chefGivePoint(chef2);
	}
	return TOURNAMENT_SUCCESS;
}
