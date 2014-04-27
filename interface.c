#include "interface.h"
#include "string.h"
#include <stdio.h>

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

void reset (Tournament * tournament, ) {
	tournamentDestroy(*tournament);
	*tournament = tournamentCreate();
	if (*tournament == NULL)  {
		printOutOfMemoryAndDie(tournament);
	}
}

void printTopDish (Tournament tournament) {
	char * chefName = strtok(NULL," ");
	Chef chef = tournamentGetChef(chefName, tournament);
	Dish topDish = chefGetTopDish(chef);
	mtmPrintTopDish(STDOUT_STREAM, chefName, dishGetName(topDish));
}

void addJudge (Tournament tournament) {
	char * nickname = strtok(NULL, " ");
	int preference = atoi(strtok(NULL, " "));
	tournamentResult result = addJudge(nickname, preference, tournament);
	if (result == TOURNAMENT_NO_MEMORY) {
		printOutOfMemoryAndDie(tournament);
	}
	if (result == TOURNAMENT_INVALID_PREFERENCE) {
		// do the error
	}
}

void printJudges (Tournament tournament) {
	char ** judges;
	int numberOfJudges;
	tournamentResult result = tournamentGetJudges(&judges, &numberOfJudges, tournament);
	if (result == TOURNAMENT_NO_JUDGES) {
		// do the error
	}
	mtmPrintAllJudges(STDOUT_STREAM, judges, numberOfJudges);
}

void printLeading (Tournament tournament) {
	Chef * leader;
	if	(leadingChef(tournament, leader) == TOURNAMENT_HAS_NO_CHEFS) {
		mtmPrintErrorMessage(STDERR_STREAM, MTM_NO_CHEFS);
		return;
	}
	mtmPrintLeadingChef(STDOUT_STREAM, chefGetName(leader), chefGetPoints(leader)); // implement getters
}

void printOutOfMemoryAndDie(Tournament tournament) {
	tournamentDestroy(tournament);
	mtmPrintErrorMessage(STDERR_STREAM, MTM_OUT_OF_MEMORY);
	exit();
}

void addChef (Tournament tournament) {
	char * name = strtok(NULL," ");
	tournamentResult result;
	result = tournamentAddChef(name, tournament);
	if (result == TOURNAMENT_OUT_OF_MEMORY) {
		printOutOfMemoryAndDie(tournament);
	}
	if (result == TOURNAMENT_CHEF_ALREADY_EXISTS) {
		mtmPrintErrorMessage(STDERR,MTM_CHEF_ALREADY_EXISTS);
	}
}

void addDish (Tournament tournament) {
	char * chefName = strtok(NULL," ");
	char * dishName = strtok(NULL," "); // why
	int dishType = atoi(strtok(NULL," "));
	int sweetness = atoi(strtok(NULL," "));
	int sourness = atoi(strtok(NULL," "));
	int saltyness = atoi(strtok(NULL," "));
	int priority = atoi(strtok(NULL," "));
	tournamentResult result = addDishToChef(name, dishType, sweetness,
								sourness, saltyness, priority, tournament);
	if (result == TOURNAMENT_OUT_OF_MEMORY) {
		printOutOfMemoryAndDie(tournament);
	}
	if (result == TOURNAMENT_NO_SUCH_CHEF) {
		// MTM_CHEF_NOT_FOUND
	}
	if (result == TOURNAMENT_BAD_DISH) {
		// MTM_INVALID_INPUT_COMMAND_PARAMETERS
	}
}
	

MtmErrorCode proccessCommand (char * line, Tournament tournament) {
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
			printCommandLineError();
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
			printCommandLineError();
		}
		return;
	}
	else {
		printCommandLineError();
	}
}

int main(int argc,char *argv[]) { // add support for input & output files
	Tournament tournament;
	char line[MAX_LEN+1];
	strcpy(line,"");
	while ((fgets(line, sizeof(line), STDIN_STREAM)) != NULL) {
		proccessCommand(line,tournament);
	}
	tournamentDestroy(tournament);
	return 0;
}

