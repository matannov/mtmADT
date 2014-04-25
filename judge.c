#include "judge.h"

static SetElement copyChef(char * chef) {
	if (chef == NULL) {
		return NULL;
	}
	char * copy = (char*)malloc(sizeof(char)*(strlen(chef)+1));
	strcpy(copy,chef);
	return copy;
}

static void destroyChef(char * chef) {
	free(chef);
}

static int compareChefs(char * first, char * second) {
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
	judge->nickname = (char*)malloc(sizeof(char)*(strlen(nickname)+1));
	judge->hatedChefs = setCreate(&copyChef,&destroyChef,&compareChefs);
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
		free(judge->nickname)
}

judgeResult addHatedChef(char * const chefName, Judge judge, bool * judgeQuits) {
	if ((chefName == NULL) || (judge == NULL) || (judgeQuits == NULL)) {
		return JUDGE_NULL_ARG;
	}
	if (badTastings == 2) {
		*judgeQuits = true;
	}
	else {
		*judgeQuits = false;
	}
	if (setAdd(chefName,judge->hatedChefs) == SET_ITEM_ALREADY_EXISTS) {
		return JUDGE_ALREADY_HATES; // is this an error?
	}
	badTastings++;
	return JUDGE_SUCCESS;
}