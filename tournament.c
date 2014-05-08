#include <stdlib.h>
#include <string.h>
#include "chef.h"
#include "judge.h"
#include "common.h"
#include "tournament.h"


static ListElement copyChef(SetElement chef) {
	return chefCopy(chef);
}

static void destroyChef(SetElement chef) {
	chefDestroy(chef);
}

static ListElement copyJudge(ListElement judge) {
	return NULL;
	//return judgeCopy(judge); ***implement***
}

static void destroyJudge(ListElement judge) {
	judgeDestroy(judge);
}

static int compareChefs(SetElement first, SetElement second) {
	bool isBetter;
	//**should be only by name**
	if (chefIsBetterRanked(first,second,&isBetter)) {
		return 1;
	}
	chefIsBetterRanked(second,first,&isBetter);
	if (isBetter) {
		return -1;
	}
	return 0;
}

Tournament tournamentCreate(tournamentResult * result) {
	Tournament tournament = (Tournament)malloc(sizeof(*tournament));
	if (tournament == NULL) {
		SAFE_ASSIGN(result,TOURNAMENT_OUT_OF_MEMORY)
		return NULL;
	}
	tournament->chefs = setCreate(&copyChef, &destroyChef, &compareChefs);
	tournament->judges = listCreate(&copyJudge, &destroyJudge);
	if ((tournament->chefs == NULL) || (tournament->judges == NULL)) {
		tournamentDestroy(tournament);
		SAFE_ASSIGN(result,TOURNAMENT_OUT_OF_MEMORY);
		return NULL;
	}
	SAFE_ASSIGN(result,TOURNAMENT_SUCCESS)
	return tournament;
}

void tournamentDestroy(Tournament tournament) {
	if (tournament != NULL) {
		setDestroy(tournament->chefs);
		listDestroy(tournament->judges);
	}
	free(tournament);
}

tournamentResult tournamentAddChef(char * const name, Tournament tournament) {
	if ((name == NULL) || (tournament == NULL)) {
		return TOURNAMENT_NULL_ARG;
	}
	ChefResult chefResult;
	Chef chef = chefCreate(name, &chefResult);
	if (chefResult == CHEF_OUT_OF_MEMORY) {
		return TOURNAMENT_OUT_OF_MEMORY;
	}
	SetResult result = setAdd(tournament->chefs, chef);
	if (result == SET_ITEM_ALREADY_EXISTS) {
		chefDestroy(chef);
		return TOURNAMENT_CHEF_ALREADY_EXISTS;
	}
	if (result == SET_OUT_OF_MEMORY) {
		chefDestroy(chef);
		return TOURNAMENT_OUT_OF_MEMORY;
	}
	return TOURNAMENT_SUCCESS;
}

tournamentResult leadingChef(Tournament tournament, Chef * leader) {
	bool isBetter;
	if ((tournament == NULL) || (leader == NULL)) {
		return TOURNAMENT_NULL_ARG;
	}
	if (setGetSize(tournament->chefs) == 0) {
		return TOURNAMENT_HAS_NO_CHEFS;
	}
	Chef best = (Chef)setGetFirst(tournament->chefs);
	SET_FOREACH(Chef,chef,tournament->chefs) {
		chefIsBetterRanked(chef,best,&isBetter);
		if (isBetter) {
			best = chef;
		}
	}
	*leader = best;
	return TOURNAMENT_SUCCESS;
}

tournamentResult tournamentAddJudge(char * const nickname, int preference, Tournament tournament) {
	if ((nickname == NULL) || (tournament == NULL)) {
		return TOURNAMENT_NULL_ARG;
	}
	JudgeResult result;
	Judge judge = judgeCreate(nickname, getInedibleFunction(), &result);
	if (result == JUDGE_BAD_PREFERENCE_RESULT) {
		return TOURNAMENT_BAD_PREFERENCE;
	}
	if ((listInsertLast(tournament->judges, judge) != LIST_SUCCESS)) {
		judgeDestroy(judge);
		return TOURNAMENT_OUT_OF_MEMORY;
	}
	return TOURNAMENT_SUCCESS;
}

tournamentResult tournamentGetTopDish(char * chefName, Tournament tournament, char ** dishName) {
	char * name;
	int nameLength;
	if ((chefName == NULL) || (tournament == NULL) || (dishName == NULL)) {
		return TOURNAMENT_NULL_ARG;
	}
	SET_FOREACH(Chef,chef,tournament->chefs){
		chefGetNameLength(chef,&nameLength);
		name = malloc(nameLength+1);
		chefGetName(chef,name);
		if (strcmp(name,chefName) == 0) {
			free(name);
			if (chefGetTopDish(chef,*dishName) == CHEF_HAS_NO_DISHES) {
				return TOURNAMENT_CHEF_HAS_NO_DISHES;
			}
			return TOURNAMENT_SUCCESS;
		}
		free(name);	
	}
	return TOURNAMENT_NO_SUCH_CHEF;
}

tournamentResult tournamentGetJudges(char *** judges, int * numberOfJudges, Tournament tournament) {
	if ((judges == NULL) || (numberOfJudges == NULL) || (tournament == NULL)) {
		return TOURNAMENT_NULL_ARG;
	}
	*judges = (char**)malloc(sizeof(char*)*listGetSize(tournament->judges));
	*numberOfJudges = 0;
	LIST_FOREACH(Judge, judge, tournament->judges) {
		(*judges)[*numberOfJudges] = judgeGetName(judge);
		(*numberOfJudges)++;
	}
	if (*numberOfJudges == 0) {
		return TOURNAMENT_HAS_NO_JUDGES;
	}
	return TOURNAMENT_SUCCESS;
}

tournamentResult addDishToChef(char * chefName, char * dishName, DishType type, int sweetness, int sourness, int saltiness, int priority, Tournament tournament) {
	Chef target;
	char * name;
	int nameLength;
	Taste taste;
	taste.sweetness = sweetness;
	taste.sourness = sourness;
	taste.saltiness = saltiness;
	DishResult result;
	Dish dish = dishCreate(dishName, type, taste, &result);
	if (result == DISH_BAD_PARAM) {
		return TOURNAMENT_BAD_DISH;
	}
	if (result == DISH_OUT_OF_MEMORY) {
		return TOURNAMENT_OUT_OF_MEMORY;
	}
	SET_FOREACH(Chef, chef, tournament->chefs) {
		chefGetNameLength(chef,&nameLength);
		name = malloc(nameLength+1);
		chefGetName(chef,name);
		if (strcmp(chefName,name) == 0) {
			target = chef;
		}
		free(name);
	}
	chefAddDish(target,dish,priority);
	return TOURNAMENT_SUCCESS;
}