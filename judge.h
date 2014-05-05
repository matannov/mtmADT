#ifndef _JUDGE_H
#define _JUDGE_H

#include "common.h"
#include "chef.h"
#include "set.h"

#define MAX_BAD_TASTING 2
#define MAX_PREFERENCE 3

typedef enum {
	JUDGE_OUT_OF_MEMORY,
	JUDGE_NULL_ARG,
	JUDGE_BAD_PREFERENCE,
	JUDGE_ALREADY_HATES,
	JUDGE_SUCCESS
} judgeResult;

typedef struct t_judge {
	char * nickname;
	int preference;
	Set hatedChefs;
	int badTastings;
} * Judge;

Judge judgeCreate(char * const nickname, int preference, judgeResult * result);
void judgeDestroy(Judge judge);
judgeResult addHatedChef(char * const chefName, Judge judge, bool * judgeQuits);
char * judgeGetName(Judge judge);


#endif // _JUDGE_H