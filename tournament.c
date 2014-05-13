#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "set.h"
#include "list.h"
#include "common.h"
#include "tournament.h"

struct tournament {
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

/* find a chef in the set. 
 * error codes: TOURNAMENT_NO_SUCH_CHEF, TOURNAMENT_OUT_OF_MEMORY */
static TournamentResult findChef(Tournament tournament, char const* name, 
	Chef* matchingChef) {

	ChefResult getResult;
	char* currentName;
	SET_FOREACH(Chef, chef, tournament->chefs) {
		getResult = chefGetName(chef, &currentName);
		if(getResult == CHEF_OUT_OF_MEMORY) {
			return TOURNAMENT_OUT_OF_MEMORY;
		}
		assert(getResult == CHEF_SUCCESS);
		if(STR_EQUALS(currentName, name)) {
			free(currentName);
			*matchingChef = chef;
			return TOURNAMENT_SUCCESS;
		}
		free(currentName);
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
	bool isBetter;
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
			free(judgesArray);
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
	free(dish);
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
	TournamentResult tournamentResult = findChef(tournament, chefName, 
		&chef);
	if(tournamentResult != TOURNAMENT_SUCCESS) {
		return tournamentResult;
	}
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
