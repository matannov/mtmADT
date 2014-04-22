#ifndef _TOURNAMENT_H
#define _TOURNAMENT_H

#include "dish.h"
#include "chef.h"
#include "judge.h"
#include "set.h"
#include "list.h"

typedef enum {
	TOURNAMENT_OUT_OF_MEMORY,
	TOURNAMENT_SUCCESS
} tournamentResult

typedef struct t_tournament {
	Set chefs;
	List judges;
} * Tournament;

Tournament tournamentCreate(tournamentResult * result);
void tournamentDestroy(Tournament tournament);
tournamentResult addChef(char * const name, int length, Tournament tournament);
tournamentResult leadingChef(Tournament tournament);
tournamentResult addJudge(char * const nickname, int preference, Tournament tournament);

#endif // _TOURNAMENT_H
