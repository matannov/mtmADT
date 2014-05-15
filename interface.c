#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "common.h"
#include "mtm_wet3.h"
#include "tournament.h"
#include "judge_preferences.h"

#define getInt(holder,param) \
	holder = strtok(NULL," \n\t"); \
	if (holder == NULL) { \
		mtmPrintErrorMessage(stdout,MTM_INVALID_INPUT_COMMAND_PARAMETERS); \
		return; \
	} \
	int param = atoi(holder);

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
	char * chefName = strtok(NULL," \n\t");
	
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
	char * nickname = strtok(NULL, " \n\t");
	char * intHolder;
	getInt(intHolder,preference);
	
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
	char * name = strtok(NULL," \n\t");
	if (name == NULL) {
		mtmPrintErrorMessage(stdout,MTM_INVALID_INPUT_COMMAND_PARAMETERS);
		return;
	}
	
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
	char * chefName = strtok(NULL," \n\t");
	char * dishName = strtok(NULL," \n\t");
	char * intHolder;
	getInt(intHolder,dishType)
	getInt(intHolder,sweetness)
	getInt(intHolder,sourness)
	getInt(intHolder,saltyness)
	getInt(intHolder,priority)
	
	Taste taste = {sweetness, sourness, saltyness};
	TournamentResult result = tournamentAddDishToChef(tournament, chefName, dishName, dishType, taste, priority);
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
		assert(result == TOURNAMENT_SUCCESS);
	}
}

static void compete(Tournament tournament) {
	char* chefName1 = strtok(NULL," \n\t");
	char* chefName2 = strtok(NULL," \n\t");
	char** resigningJudges;
	int resigningCount;
	bool firstChefWins, secondChefWins;
	TournamentResult result = tournamentCompete(tournament, chefName1, 
		chefName2, &resigningJudges, &resigningCount, &firstChefWins, 
		&secondChefWins);
	switch(result) {
	case TOURNAMENT_OUT_OF_MEMORY:
		handleOutOfMemory(tournament);
	case TOURNAMENT_NO_SUCH_CHEF:
		mtmPrintErrorMessage(stderr, MTM_CHEF_NOT_FOUND);
		return;
	case TOURNAMENT_SAME_CHEF:
		mtmPrintErrorMessage(stderr, MTM_SAME_CHEF);
		return;
	case TOURNAMENT_CHEF_HAS_NO_DISHES:
		mtmPrintErrorMessage(stderr, MTM_CHEF_HAS_NO_DISHES);
		return;
	default:
		assert(result == TOURNAMENT_SUCCESS || 
			result == TOURNAMENT_HAS_NO_JUDGES);
	}
	for(int i=0; i<resigningCount; i++) {
		mtmPrintJudgeResignationMessage(stdout,resigningJudges[i]);
		free(resigningJudges[i]);
	}
	free(resigningJudges);
	if(result == TOURNAMENT_HAS_NO_JUDGES) {
		mtmPrintErrorMessage(stderr,MTM_NO_JUDGES);
		return;
	}
	if(firstChefWins) {
		mtmPrintWinningAndLosingChefs(stdout, chefName1, chefName2);
	} else if(secondChefWins) {
		mtmPrintWinningAndLosingChefs(stdout, chefName2, chefName2);
	} else {
		mtmPrintTieBetweenTwoChefs(stdout, chefName1, chefName2);
	}
}
	
static void proccessCommand (char * line, Tournament tournament) {
	char * primaryCommand = strtok(line," \n\t");
	char * secondaryCommand = strtok(NULL," \n\t");
	if (primaryCommand == NULL) {
		return;
	}
	if (strchr(primaryCommand,'#') == primaryCommand) {
		return;
	}
	if (secondaryCommand == NULL) {
		mtmPrintErrorMessage(stderr, MTM_INVALID_INPUT_COMMAND_PARAMETERS);
		return;
	}
	if (STR_EQUALS(primaryCommand,"reset")) {	
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


