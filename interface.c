#include "interface.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>

/* todo list:
	reset V
	addChef V
	addDish V
	compete 
	printLeading V
	printTopDish V
	addJudge V
	printJudges V
	printOutOfMemoryAndDie V
*/

void printOutOfMemoryAndDie(Tournament tournament) {
	tournamentDestroy(tournament);
	mtmPrintErrorMessage(stderr, MTM_OUT_OF_MEMORY);
	exit(0);
}

static void printCommandLineErrorAndDie(Tournament tournament) {
	tournamentDestroy(tournament);
	mtmPrintErrorMessage(stdout,MTM_INVALID_COMMAND_LINE_PARAMETERS);
	exit(0);
}

void reset (Tournament * tournament) {
	tournamentDestroy(*tournament);
	*tournament = tournamentCreate(NULL);
	if (*tournament == NULL)  {
		printOutOfMemoryAndDie(*tournament);
	}
}

void printTopDish (Tournament tournament) {
	char * chefName = strtok(NULL," ");
	if (chefName == NULL) {
		printCommandLineErrorAndDie(tournament);
	}
	char * dishName;
	tournamentResult result = tournamentGetTopDish(chefName, tournament, &dishName);
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

void addJudge (Tournament tournament) {
	char * nickname = strtok(NULL, " ");
	int preference = atoi(strtok(NULL, " "));
	tournamentResult result = tournamentAddJudge(nickname, preference, tournament);
	if (result == TOURNAMENT_OUT_OF_MEMORY) {
		printOutOfMemoryAndDie(tournament);
	}
	if (result == TOURNAMENT_BAD_PREFERENCE) {
		// do the error
	}
}

void printJudges (Tournament tournament) {
	char ** judges;
	int numberOfJudges;
	tournamentResult result = tournamentGetJudges(&judges, &numberOfJudges, tournament);
	if (result == TOURNAMENT_HAS_NO_JUDGES) {
		// do the error
	}
	mtmPrintAllJudges(stdout, judges, numberOfJudges);
}

void printLeading (Tournament tournament) {
	Chef leader;
	if	(leadingChef(tournament, &leader) == TOURNAMENT_HAS_NO_CHEFS) {
		mtmPrintErrorMessage(stderr, MTM_NO_CHEFS);
		return;
	}
	mtmPrintLeadingChef(stdout, chefGetName(leader), chefGetPoints(leader)); // implement getters
}

void addChef (Tournament tournament) {
	char * name = strtok(NULL," ");
	tournamentResult result;
	result = tournamentAddChef(name, tournament);
	if (result == TOURNAMENT_OUT_OF_MEMORY) {
		printOutOfMemoryAndDie(tournament);
	}
	if (result == TOURNAMENT_CHEF_ALREADY_EXISTS) {
		mtmPrintErrorMessage(stderr,MTM_CHEF_ALREADY_EXISTS);
	}
}

void addDish (Tournament tournament) {
	char * chefName = strtok(NULL," ");
	char * dishName = strtok(NULL," ");
	int dishType = atoi(strtok(NULL," "));
	int sweetness = atoi(strtok(NULL," "));
	int sourness = atoi(strtok(NULL," "));
	int saltyness = atoi(strtok(NULL," "));
	int priority = atoi(strtok(NULL," "));
	tournamentResult result = addDishToChef(chefName, dishName, dishType, sweetness,
								sourness, saltyness, priority, tournament);
	if (result == TOURNAMENT_OUT_OF_MEMORY) {
		printOutOfMemoryAndDie(tournament);
	}
	if (result == TOURNAMENT_NO_SUCH_CHEF) {
		mtmPrintErrorMessage(stderr, MTM_CHEF_NOT_FOUND);
	}
	if (result == TOURNAMENT_BAD_DISH) {
		printCommandLineErrorAndDie(tournament);
	}
}
	
void proccessCommand (char * line, Tournament tournament) {
	char * primaryCommand = strtok(line," ");
	char * secondaryCommand = strtok(NULL," ");
	if (strcmp(primaryCommand,"reset") == 0) {	
		
		reset(&tournament);
	}
	else if (strcmp(primaryCommand,"chef") == 0) {
		if (strcmp(secondaryCommand,"add") == 0) {
			addChef(tournament);
		}
		else if (strcmp(secondaryCommand,"add-dish") == 0) {
			addDish(tournament);
		}
		else if (strcmp(secondaryCommand,"compete") == 0) {
			//compete(tournament); implement this
		}
		else if (strcmp(secondaryCommand,"leading") == 0) {
			printLeading(tournament);
		}
		else if (strcmp(secondaryCommand,"top-dish") == 0) {
			printTopDish(tournament);
		}
		else {
			printCommandLineErrorAndDie(tournament);
		}
		return;
	}
	else if (strcmp(primaryCommand,"judge") == 0) {
		if (strcmp(secondaryCommand,"add") == 0) {
			addJudge(tournament);
		}
		else if (strcmp(secondaryCommand,"print") == 0) {
			printJudges(tournament);
		}
		else {
			printCommandLineErrorAndDie(tournament);
		}
		return;
	}
	else {
		printCommandLineErrorAndDie(tournament);
	}
}

int main(int argc,char *argv[]) { // add support for input & output files
	tournamentResult result;
	Tournament tournament = tournamentCreate(&result);
	if (result == TOURNAMENT_OUT_OF_MEMORY) {
		printOutOfMemoryAndDie(tournament);
	}
	char line[MAX_LEN+1];
	strcpy(line,"");
	while ((fgets(line, sizeof(line), stdin)) != NULL) {
		proccessCommand(line,tournament);
	}
	tournamentDestroy(tournament);
	return 0;
}

