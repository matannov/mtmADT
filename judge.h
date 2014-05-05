#ifndef _JUDGE_H
#define _JUDGE_H

#include <stdbool.h>
#include "dish.h"

#define JUDGE_BAD_TASTING_LIMIT 2

#define JUDGE_PREFERENCE_MIN 0
#define JUDGE_PREFERENCE_MAX (JUDGE_PREFERENCE_END-1)
typedef enum {
	JUDGE_PREFER_FIRST = JUDGE_PREFERENCE_MIN,
	JUDGE_PREFER_SECOND,
	JUDGE_TIE,
	JUDGE_PREFERENCE_END
} JudgePreference;

typedef JudgePreference (*JudgeByPreference)(Dish dish1, Dish dish2, 
	bool isUnedible1, bool isUnedible2);

typedef enum {
	JUDGE_SUCCESS,
	JUDGE_OUT_OF_MEMORY,
	JUDGE_NULL_ARGUMENT,
	JUDGE_ALREADY_QUITTED,
	JUDGE_BAD_PREFERENCE_RESULT
} JudgeResult;

typedef struct judge *Judge;

/*
 * Create a new judge with given nickname and preference function.
 *
 * @param nickname Nickname for new judge. The string is copied.
 * @param result Result success or error code.
 * 	If "result" is null, it is ignored and no code is returned.
 *	Error codes: JUDGE_NULL_ARGUMENT, CHEF_OUT_OF_MEMORY
 * @return The new chef, NULL in case of error.
 */
Judge judgeCreate(char const* nickname, JudgeByPreference judgeByPreference, 
	JudgeResult* result);

/*
 * Destroy given judge.
 *
 * @param judge Judge to destroy, if NULL does nothing.
 */
void judgeDestroy(Judge judge);

/*
 * Create a copy of an existing judge.
 *
 * @param source The judge to copy.
 * @return Judge copy, NULL in case of NULL argument or out of memory.
 */
Judge judgeCopy(Judge source);

/*
 * Get judge nickname.
 *
 * @param judge Judge to get it's nickname
 * @param buffer Name will be written here. Make sure there is enough space.
 * @return Result success or error code.
 *	Error codes: JUDGE_NULL_ARGUMENT
 */
JudgeResult judgeGetNickname(Judge judge, char* buffer);

/*
 * Judge betweem two dishes.
 *
 * If a dish is unedible, the judge will start to hate it's chef.
 * If a judges hates one chef, he will always rule in favor of the chef he
 * doesn't hate. If a judge hates both chefs or non of them, he will rule
 * according to his taste preference (the preference function given at creation)
 * and if the preference is equal, he will choose the dish of the chef whos
 * name is prior lexicographically (must be a winner).
 *
 * @param judge Judge that is judging
 * @param dish1 First dish.
 * @param dish2 Second dish.
 * @param chefName1 Name of first chef.
 * @param chefName2 Name of second chef.
 * @param dish1Wins True if dish1 wins, false if dish2 wins
 * @param judgeQuits True if the judge quits. 
 *	A judge quits only after he finished judging between the two dishes.
 *      A judge will quit if he tasted too many unedible dishes.
 *	After a judge quits, he can no longer judge between dishes. 
 * @return Result success or error code.
 *	Error codes: JUDGE_NULL_ARGUMENT, JUDGE_ALREADY_QUITTED,
 *	JUDGE_BAD_PREFERENCE_RESULT (preference function returned invalid value)
 */
JudgeResult judgeJudgeDishes(Judge judge, Dish dish1, Dish dish2, 
	char* chefName1, char* chefName2, bool* dish1Wins, bool* judgeQuits);


#endif // _JUDGE_H