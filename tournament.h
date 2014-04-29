#ifndef _TOURNAMENT_H
#define _TOURNAMENT_H

#include "commonDefs.h"
#include "dish.h"
#include "chef.h"
#include "judge.h"
#include "set.h"
#include "list.h"

typedef enum {
	TOURNAMENT_OUT_OF_MEMORY,
	TOURNAMENT_NULL_ARG,
	TOURNAMENT_BAD_PREFERENCE,
	TOURNAMENT_BAD_DISH,
	TOURNAMENT_NO_SUCH_CHEF,
	TOURNAMENT_CHEF_HAS_NO_DISHES,
	TOURNAMENT_CHEF_ALREADY_EXISTS,
	TOURNAMENT_HAS_NO_CHEFS,
	TOURNAMENT_HAS_NO_JUDGES,
	TOURNAMENT_SUCCESS
} tournamentResult;

typedef struct t_tournament {
	Set chefs;
	List judges;
} * Tournament;

Tournament tournamentCreate(tournamentResult * result);
void tournamentDestroy(Tournament tournament);
tournamentResult tournamentAddChef(char * const name, Tournament tournament);
tournamentResult leadingChef(Tournament tournament, Chef * leader);
tournamentResult tournamentAddJudge(char * const nickname, int preference, Tournament tournament);
tournamentResult addDishToChef(char * chefName, char * dishName, DISH_TYPE type, int sweetness, int sourness, int saltiness, int priority, Tournament tournament); // implement
tournamentResult tournamentGetTopDish(char * chefName, Tournament tournament, char ** dishName);
tournamentResult tournamentGetJudges(char *** judges, int * numberOfJudges, Tournament 	tournament);
#endif // _TOURNAMENT_H
