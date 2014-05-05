#include <stdlib.h>
#include <string.h>
#include "chef.h"
#include "judge.h"
#include "commonDefs.h"
#include "tournament.h"


static ListElement copyChef(SetElement chef) {
	return chefCopy(chef);
}

static void destroyChef(SetElement chef) {
	chefDestroy(chef);
}

static int compareChefs(SetElement first, SetElement second) {
	//**should be only by name**
	if (chefIsBetterRanked(first,second)) {
		return 1;
	}
	if (chefIsBetterRanked(second,first)) {
		return -1;
	}
	return 0;
}

static ListElement copyJudge(ListElement judge) {
	return NULL;
	//return judgeCopy(judge); ***implement***
}

static void destroyJudge(ListElement judge) {
	judgeDestroy(judge);
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
	if ((tournament == NULL) || (leader == NULL)) {
		return TOURNAMENT_NULL_ARG;
	}
	if (setGetSize(tournament->chefs) == 0) {
		return TOURNAMENT_HAS_NO_CHEFS;
	}
	Chef best = (Chef)setGetFirst(tournament->chefs);
	SET_FOREACH(Chef,chef,tournament->chefs) {
		if (chefIsBetterRanked(chef,best)) {
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
	judgeResult result;
	Judge judge = judgeCreate(nickname, preference, &result);
	if (result == JUDGE_BAD_PREFERENCE) {
		return TOURNAMENT_BAD_PREFERENCE;
	}
	if ((listInsertLast(tournament->judges, judge) != LIST_SUCCESS)) {
		judgeDestroy(judge);
		return TOURNAMENT_OUT_OF_MEMORY;
	}
	return TOURNAMENT_SUCCESS;
}

tournamentResult tournamentGetTopDish(char * chefName, Tournament tournament, char ** dishName) {
	if ((chefName == NULL) || (tournament == NULL) || (dishName == NULL)) {
		return TOURNAMENT_NULL_ARG;
	}
	SET_FOREACH(Chef,chef,tournament->chefs){
		if (strcmp(chefGetName(chef),chefName) == 0) {
			if (chefGetTopDish(chef,dishName) == CHEF_HAS_NO_DISHES) {
				return TOURNAMENT_CHEF_HAS_NO_DISHES;
			}
			return TOURNAMENT_SUCCESS;
		}
	}
	return TOURNAMENT_NO_SUCH_CHEF;
}