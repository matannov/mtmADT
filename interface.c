#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "common.h"
#include "mtm_wet3.h"
#include "tournament.h"
#include "judge_preferences.h"

#define getInt(holder,param) \
	holder = strtok(NULL," \n"); \
	if (holder == NULL) { \
		mtmPrintErrorMessage(stdout,MTM_INVALID_INPUT_COMMAND_PARAMETERS); \
		return; \
	} \
	printf("got %s",holder); \
	int param = atoi(holder);
	
#define CHECK_OVERFLOW \
		if (strtok(NULL," \n") != NULL) { \
		mtmPrintErrorMessage(stderr, MTM_INVALID_INPUT_COMMAND_PARAMETERS); \
		return; \
	}

/* called to handle an out of memory error */
static void handleOutOfMemory(Tournament tournament) {
	mtmPrintErrorMessage(stdout, MTM_OUT_OF_MEMORY);
	tournamentDestroy(tournament);
	exit(0);
}

static void reset (Tournament * tournament) {
	tournamentDestroy(*tournament);
	*tournament = tournamentCreate(NULL);
	if (*tournament == NULL)  {
		handleOutOfMemory(*tournament);
	}
}

static void printTopDish (Tournament tournament) {
	char * chefName = strtok(NULL," \n");
	CHECK_OVERFLOW;
	if (chefName == NULL) {
		mtmPrintErrorMessage(stderr, MTM_INVALID_INPUT_COMMAND_PARAMETERS);
		return;
	}
	char * dishName;
	TournamentResult result = tournamentGetTopDish(tournament, chefName, &dishName);
	if (result == TOURNAMENT_NO_SUCH_CHEF) {
		mtmPrintErrorMessage(stdout,MTM_CHEF_NOT_FOUND);
		return;
	}
	if (result == TOURNAMENT_CHEF_HAS_NO_DISHES) {
		mtmPrintErrorMessage(stdout,MTM_CHEF_HAS_NO_DISHES);
		return;
	}
	mtmPrintTopDish(stdout, chefName, dishName);
}

static void addJudge (Tournament tournament) {
	char * nickname = strtok(NULL, " \n");
	char * intHolder;
	getInt(intHolder,preference);
	CHECK_OVERFLOW;
	JudgeByPreference judgeByPreference;
	switch(preference) {
	case 1:
		judgeByPreference = judgeByPreference1;
		break;
	case 2:
		judgeByPreference = judgeByPreference2;
		break;
	case 3:
		judgeByPreference = judgeByPreference3;
		break;
	default:
		mtmPrintErrorMessage(stdout,MTM_INVALID_INPUT_COMMAND_PARAMETERS);
		return;	
	}
	TournamentResult result = tournamentAddJudge(tournament, nickname, judgeByPreference);
	if (result == TOURNAMENT_OUT_OF_MEMORY) {
		handleOutOfMemory(tournament);
	}
}

static void printJudges (Tournament tournament) {
	CHECK_OVERFLOW;
	char ** judges;
	int numberOfJudges;
	TournamentResult result = tournamentGetJudges(tournament, &judges, &numberOfJudges);
	if(result == TOURNAMENT_HAS_NO_JUDGES) {
		mtmPrintErrorMessage(stderr,MTM_NO_JUDGES);
		return;
	}
	mtmPrintAllJudges(stdout, judges, numberOfJudges);
}

static void printLeading (Tournament tournament) {
	CHECK_OVERFLOW;
	Chef leader;
	if(tournamentLeadingChef(tournament, &leader) == TOURNAMENT_HAS_NO_CHEFS) {
		mtmPrintErrorMessage(stderr, MTM_NO_CHEFS);
		return;
	}
	int points;
	char* name;
	chefGetPoints(leader, &points);
	chefGetName(leader, &name);
	mtmPrintLeadingChef(stdout, name, points);
	free(name);
}

static void addChef (Tournament tournament) {
	char * name = strtok(NULL," \n");
	if (name == NULL) {
		mtmPrintErrorMessage(stdout,MTM_INVALID_INPUT_COMMAND_PARAMETERS);
		return;
	}
	CHECK_OVERFLOW;
	TournamentResult result;
	result = tournamentAddChef(tournament, name);
	if (result == TOURNAMENT_OUT_OF_MEMORY) {
		handleOutOfMemory(tournament);
	}
	if (result == TOURNAMENT_CHEF_ALREADY_EXISTS) {
		mtmPrintErrorMessage(stderr,MTM_CHEF_ALREADY_EXISTS);
	}
}

static void addDish (Tournament tournament) {
	char * chefName = strtok(NULL," \n");
	char * dishName = strtok(NULL," \n");
	char * intHolder;
	getInt(intHolder,dishType)
	getInt(intHolder,sweetness)
	getInt(intHolder,sourness)
	getInt(intHolder,saltyness)
	getInt(intHolder,priority)
	CHECK_OVERFLOW;
	Taste taste = {sweetness, sourness, saltyness};
	TournamentResult result = tournamentAddDishToChef(tournament, chefName, dishName, dishType, taste, priority);
	if(result != TOURNAMENT_SUCCESS) {
		switch(result) {
		case TOURNAMENT_OUT_OF_MEMORY:
			handleOutOfMemory(tournament);
			break;
		case TOURNAMENT_NO_SUCH_CHEF:
			mtmPrintErrorMessage(stderr, MTM_CHEF_NOT_FOUND);
			break;
		case TOURNAMENT_BAD_PARAM:
			mtmPrintErrorMessage(stderr, MTM_INVALID_INPUT_COMMAND_PARAMETERS);
			break;
		default:
			assert(false); //unhandled error
		}
	}
}

static void compete(Tournament tournament) {
	char * chefA = strtok(NULL," \n");
	char * chefB = strtok(NULL," \n");
	if (chefB == NULL) {
		mtmPrintErrorMessage(stderr, MTM_INVALID_INPUT_COMMAND_PARAMETERS);
		return;
	}
	CHECK_OVERFLOW;
	char ** resigningJudges;
	int numberJudgesResigned;
	bool firstChefWins, secondChefWins;
	TournamentResult result = tournamentCompete(tournament, 
	chefA, chefB, &resigningJudges,
	&numberJudgesResigned, &firstChefWins, &secondChefWins);
	if (result == TOURNAMENT_NO_SUCH_CHEF) {
		mtmPrintErrorMessage(stderr,MTM_CHEF_NOT_FOUND);
		return;
	}
	if (strcmp(chefA,chefB) == 0) {
		mtmPrintErrorMessage(stderr,MTM_SAME_CHEF);
		return;
	}
	if (result == TOURNAMENT_HAS_NO_JUDGES) {
		mtmPrintErrorMessage(stderr,MTM_NO_JUDGES);
		return;
	}
	if (result == TOURNAMENT_CHEF_HAS_NO_DISHES) {
		mtmPrintErrorMessage(stderr,MTM_CHEF_HAS_NO_DISHES);
		return;
	}
	if (numberJudgesResigned > 0) {
		for (int i=0;i<numberJudgesResigned;i++) {
			mtmPrintJudgeResignationMessage(stdout,resigningJudges[i]);
			free(resigningJudges[i]);
		}
	}
	free(resigningJudges);
	if (firstChefWins) {
		mtmPrintWinningAndLosingChefs(stdout, chefA, chefB);
	}
	else if (secondChefWins) {
		mtmPrintWinningAndLosingChefs(stdout, chefB, chefA);
	}
	else {
		mtmPrintTieBetweenTwoChefs(stdout, chefA, chefB);
	}
}
	
static void proccessCommand (char * line, Tournament tournament) {
	char * primaryCommand = strtok(line," \n");
	char * secondaryCommand = strtok(NULL," \n");
	if ((primaryCommand == NULL) || (secondaryCommand == NULL)) {
		mtmPrintErrorMessage(stderr, MTM_INVALID_INPUT_COMMAND_PARAMETERS);
		return;
	}
	if (STR_EQUALS(primaryCommand,"reset")) {	
		printf("reseting...\n");
		reset(&tournament);
		return;
	}

	//todo change to static array
	if (STR_EQUALS(primaryCommand,"chef")) {
		if (STR_EQUALS(secondaryCommand,"add")) {
			addChef(tournament);
		}
		else if (STR_EQUALS(secondaryCommand,"add-dish")) {
			addDish(tournament);
		}
		else if (STR_EQUALS(secondaryCommand,"compete")) {
			compete(tournament);
		}
		else if (STR_EQUALS(secondaryCommand,"leading")) {
			printLeading(tournament);
		}
		else if (STR_EQUALS(secondaryCommand,"top-dish")) {
			printTopDish(tournament);
		}
		else {
			mtmPrintErrorMessage(stderr, MTM_INVALID_INPUT_COMMAND_PARAMETERS);
		}
	}
	else if (STR_EQUALS(primaryCommand,"judge")) {
		if (STR_EQUALS(secondaryCommand,"add")) {
			addJudge(tournament);
		}
		else if (STR_EQUALS(secondaryCommand,"print")) {
			printJudges(tournament);
		}
		else {
			mtmPrintErrorMessage(stderr, MTM_INVALID_INPUT_COMMAND_PARAMETERS);
		}
	}
	else {
		mtmPrintErrorMessage(stderr, MTM_INVALID_INPUT_COMMAND_PARAMETERS);
	}
}

int main(int argc,char *argv[]) { // add support for input & output files
	TournamentResult result;
	Tournament tournament = tournamentCreate(&result);
	if (result == TOURNAMENT_OUT_OF_MEMORY) {
		handleOutOfMemory(tournament);
	}
	char line[MAX_LEN+1];
	strcpy(line,"");
	while ((fgets(line, sizeof(line), stdin)) != NULL) {
		proccessCommand(line,tournament);
	}
	tournamentDestroy(tournament);
	return 0;
}


