#ifndef _JUDGE_PREFERENCES_H
#define _JUDGE_PREFERENCES_H

#include "judge.h"
#include "dish.h"

/* 
 * Judge preference functions.
 * These functions are passed to judgeCreate and affect the 
 * judge's taste preference.
 * 
 */

/* implements judge preference 1 */
JudgePreference judgeByPreference1(Dish dish1, Dish dish2, 
	bool isUnedible1, bool isUnedible2);

/* implements judge preference 2 */
JudgePreference judgeByPreference2(Dish dish1, Dish dish2, 
	bool isUnedible1, bool isUnedible2);

/* implements judge preference 3 */
JudgePreference judgeByPreference3(Dish dish1, Dish dish2, 
	bool isUnedible1, bool isUnedible2);

#endif // _JUDGE_PREFERENCES_H
