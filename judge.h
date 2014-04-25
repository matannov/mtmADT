#ifndef _JUDGE_H
#define _JUDGE_H

#include "chef.h"

typedef enum judgeResult {
	JUDGE_OUT_OF_MEMORY,
	JUDGE_NULL_ARG,
	JUDGE_ALREADY_HATES,
	JUDGE_SUCCESS
};

typedef struct t_judge {
	char * nickname;
	int preference;
	Set hatedChefs;
	int badTastings;
} * Judge;

Judge judgeCreate(char * const nickname, int preference, judgeResult * result);
void judgeDestroy(Judge judge);
judgeResult addHatedChef(char * const chefName, Judge judge, bool * judgeQuits);


#endif // _JUDGE_H