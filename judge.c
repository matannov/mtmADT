#include <stdlib.h>
#include <string.h>
#include "judge.h"

static SetElement copyChef(void* chef) {
	if (chef == NULL) {
		return NULL;
	}
	char * copy = malloc(strlen(chef)+1);
	strcpy(copy, chef);
	return copy;
}

static void destroyChef(void* chef) {
	free(chef);
}

static int compareChefs(void* first, void* second) {
	return strcmp(first,second);
}

Judge judgeCreate(char * const nickname, int preference, judgeResult * result) {
	Judge judge = (Judge)malloc(sizeof(*judge));
	if (judge == NULL) {
		if (result != NULL) {
			*result = JUDGE_OUT_OF_MEMORY;
		}
		return judge;
	}
	if (nickname == NULL) {
		if (result != NULL) {
			*result = JUDGE_NULL_ARG;
		}
		return judge;
	}
	judge->nickname = malloc(strlen(nickname)+1);
	judge->hatedChefs = setCreate(&copyChef, &destroyChef, &compareChefs);
	if ((judge->nickname == NULL) || (judge->hatedChefs == NULL)) {
		setDestroy(judge->hatedChefs);
		if (result != NULL) {
			*result = JUDGE_OUT_OF_MEMORY;
		}
		return judge;
	}
	strcpy(judge->nickname, nickname);
	judge->preference = preference;
	if (result != NULL) {
		*result = JUDGE_SUCCESS;
	}
	judge->badTastings = 0;
	return judge;
}

void judgeDestroy(Judge judge) {
	if (judge != NULL) {
		free(judge->nickname);
	}
}

judgeResult addHatedChef(char * const chefName, Judge judge, bool * judgeQuits) {
	if ((chefName == NULL) || (judge == NULL) || (judgeQuits == NULL)) {
		return JUDGE_NULL_ARG;
	}
	if (judge->badTastings == MAX_BAD_TASTING) {
		*judgeQuits = true;
	}
	else {
		*judgeQuits = false;
	}
	if (setAdd(judge->hatedChefs, chefName) == SET_ITEM_ALREADY_EXISTS) {
		return JUDGE_ALREADY_HATES; // is this an error?
	}
	judge->badTastings++;
	return JUDGE_SUCCESS;
}