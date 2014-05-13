#ifndef _TOURNAMENT_H
#define _TOURNAMENT_H

#include "chef.h"
#include "judge.h"

typedef enum {
	TOURNAMENT_SUCCESS,
	TOURNAMENT_OUT_OF_MEMORY,
	TOURNAMENT_NULL_ARGUMENT,
	TOURNAMENT_BAD_PARAM,
	TOURNAMENT_NO_SUCH_CHEF,
	TOURNAMENT_CHEF_HAS_NO_DISHES,
	TOURNAMENT_CHEF_ALREADY_EXISTS,
	TOURNAMENT_HAS_NO_CHEFS,
	TOURNAMENT_HAS_NO_JUDGES
} TournamentResult;

typedef struct tournament *Tournament;

/*
 * Create a new tournament.
 *
 * @param result Result success or error code.
 *	If "result" is null, no code is returned.
 *	Error codes: TOURNAMENT_OUT_OF_MEMORY
 * @return The new tournament, NULL in case of error.
 */
Tournament tournamentCreate(TournamentResult* result);

/*
 * Destroy given tournament.
 *
 * @param tournament Tournament to destroy, if NULL does nothing.
 */
void tournamentDestroy(Tournament tournament);

/*
 * Add a new chef to the tournament.
 *
 * @param tournament Tournament to add to.
 * @param name Name of new chef.
 * @return Result success or error code.
 *	Error codes: TOURNAMENT_NULL_ARGUMENT, TOURNAMENT_OUT_OF_MEMORY,
 *	TOURNAMENT_CHEF_ALREADY_EXISTS
 */
TournamentResult tournamentAddChef(Tournament tournament, char const* name);

/*
 * Get the leading chef of the tournament. The actual chef is returned.
 *
 * @param tournament Tournament to get from.
 * @param leader Leading chef will be set here. 
 * @return Result success or error code.
 *	Error codes: TOURNAMENT_NULL_ARGUMENT, TOURNAMENT_HAS_NO_CHEFS
 */
TournamentResult tournamentLeadingChef(Tournament tournament, Chef* leader);

/*
 * Add a new judge to the tournament.
 *
 * @param tournament Tournament to add to.
 * @param nickname Nickname of new judge.
 * @param judgeByPreference Preference function for the judge.
 * @return Result success or error code.
 *	Error codes: TOURNAMENT_NULL_ARGUMENT, TOURNAMENT_OUT_OF_MEMORY,
 *	TOURNAMENT_CHEF_ALREADY_EXISTS
 */
TournamentResult tournamentAddJudge(Tournament tournament, char const* nickname, 
	JudgeByPreference judgeByPreference);

/*
 * Get an array with nicknames of all judges.
 *
 * @param tournament Tournament to get from.
 * @param judges Name Pointer to an array of strings will be written here.
 *	User is responsible to free all the strings and the array itself.
 * @param numberOfJudges Number of judges will be written here.
 * @return Result success or error code.
 *	Error codes: TOURNAMENT_NULL_ARGUMENT, TOURNAMENT_OUT_OF_MEMORY,
 *	TOURNAMENT_NO_SUCH_CHEF, TOURNAMENT_CHEF_HAS_NO_DISHES
 */
TournamentResult tournamentGetJudges(Tournament tournament, char*** judges, 
	int* numberOfJudges);

/*
 * Add a new dish to chef in the tournament.
 *
 * @param tournament Tournament to add to.
 * @param chefName Name of the chef in tournament.
 * @param name Name for new dish. The string is copied.
 * @param type Type of dish.
 * @param taste Taste params of dish
 * @param priority Priority of the dish.
 * @return Result success or error code.
 *	Error codes: TOURNAMENT_NULL_ARGUMENT, TOURNAMENT_OUT_OF_MEMORY,
 *	TOURNAMENT_NO_SUCH_CHEF, TOURNAMENT_BAD_PARAM
 */
TournamentResult tournamentAddDishToChef(Tournament tournament, 
	char const* chefName, char const* dishName, DishType type, Taste taste, 
	int priority);

/*
 * Get the name of top priority dish from a specific chef.
 * If some dishes have the same priority, the dish that was added first is 
 * returned.
 *
 * @param tournament Tournament to get from.
 * @param chefName Name of the chef in tournament.
 * @param dishName String pointer will be written here. 
 * 	User is responsible to free this memory.
 * @return Result success or error code.
 *	Error codes: TOURNAMENT_NULL_ARGUMENT, TOURNAMENT_OUT_OF_MEMORY,
 *	TOURNAMENT_NO_SUCH_CHEF, TOURNAMENT_CHEF_HAS_NO_DISHES
 */
TournamentResult tournamentGetTopDish(Tournament tournament, 
	char const* chefName, char** dishName);

#endif // _TOURNAMENT_H
