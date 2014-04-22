#ifndef _JUDGE_H
#define _JUDGE_H

#include "chef.h"

typedef enum judgeResult {
	JUDGE_OUT_OF_MEMORY,
	JUDGE_SUCCESS
};

typedef struct t_judge {
	char * nickname;
	int preference;
	Set hatedChefs;
} * Judge;

Judge judgeCreate(char * const nickname, int preference, judgeResult * result);
void judgeDestroy(Judge judge);
judgeResult addHatedChef(char * const chefName, Judge judge);


#endif // _JUDGE_H