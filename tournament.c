#include <stdlib.h>
#include "tournament.h"

static ListElement copyChef(ListElement chef) {
	return chefCopy(chef);
}

static void destroyChef(ListElement chef) {
	chefDestroy(chef);
}

static ListElement copyJudge(ListElement judge) {
	return NULL;
	//return judgeCopy(judge); ***implement***
}

static void destroyJudge(ListElement judge) {
	judgeDestroy(judge);
}

static int compareChefs(ListElement first, ListElement second) {
	if (chefIsBetter(first,second)) {
		return 1;
	}
	if (chefIsBetter(second,first)) {
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

tournamentResult addChef(char * const name, Tournament tournament) {
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
		if (chefIsBetter(chef,best)) {
			best = chef;
		}
	}
	*leader = best;
	return TOURNAMENT_SUCCESS;
}

tournamentResult addJudge(char * const nickname, int preference, Tournament tournament) {
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