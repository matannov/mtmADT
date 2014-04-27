#ifndef _TOURNAMENT_H
#define _TOURNAMENT_H

#define SAFE_ASSIGN(ptr,val) \
	if ((ptr) != NULL) { \
		*(ptr) = val; \
	}

#include "dish.h"
#include "chef.h"
#include "judge.h"
#include "set.h"
#include "list.h"

typedef enum {
	TOURNAMENT_OUT_OF_MEMORY,
	TOURNAMENT_NULL_ARG,
	TOURNAMENT_CHEF_ALREADY_EXISTS,
	TOURNAMENT_HAS_NO_CHEFS,
	TOURNAMENT_SUCCESS
} tournamentResult;

typedef struct t_tournament {
	Set chefs;
	List judges;
} * Tournament;

Tournament tournamentCreate(tournamentResult * result);
void tournamentDestroy(Tournament tournament);
tournamentResult addChef(char * const name, Tournament tournament);
tournamentResult leadingChef(Tournament tournament, Chef * leader);
tournamentResult addJudge(char * const nickname, int preference, Tournament tournament);
tournamentResult addDishToChef(char * chefName, DISH_PARAMETERS, Tournament tournament); // implement

#endif // _TOURNAMENT_H
