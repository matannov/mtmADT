#ifndef MTM_WET3_H_
#define MTM_WET3_H_

#include <stdio.h> // For FILE*
				   // You should avoid including unnecessary header files

/**
 * Maximal input line length
 */
#define MAX_LEN 300


/**
 * This type defines all errors for the system.
 * These codes should be used for calling mtmPrintErrorMessage to report
 * error messages.
 *
 * Notice that after calling mtmPrintErrorMessage with one of the first
 * three messages you should abort the program according to the exercise's
 * rules.
 */
typedef enum {
	MTM_OUT_OF_MEMORY, // You should exit program after this error
	MTM_INVALID_COMMAND_LINE_PARAMETERS, // You should exit the program after
										 // this error
	MTM_CANNOT_OPEN_FILE, // You should exit program after this error
	MTM_INVALID_INPUT_COMMAND_PARAMETERS,
	MTM_NO_CHEFS,
	MTM_NO_JUDGES,
	MTM_CHEF_NOT_FOUND,
	MTM_SAME_CHEF,
	MTM_CHEF_HAS_NO_DISHES,
	MTM_CHEF_ALREADY_EXISTS,
} MtmErrorCode;


/**
 * mtmPrintErrorMessage - prints an error message into the given output channel
 * @param channel - File descriptor for the error channel
 * @param code - Error code to print message for.
 */
void mtmPrintErrorMessage(FILE* channel, MtmErrorCode code);


/**
 * Prints a message about a judge furiously resigning.
 * @param output - File descriptor for the output channel
 * @param nickname - The nickname of the judge.
 */
void mtmPrintJudgeResignationMessage(FILE* output, char* nickname);


/**
 * Prints a message about the outcome of a competition between two chefs, in
 * which one chef won (and earned a gold point) and another lost.
 * @param output - File descriptor for the output channel.
 * @param winningChef - The name of the chef that won.
 * @param lossingChef - The name of the chef that lost.
 */
void mtmPrintWinningAndLosingChefs(FILE* output, char* winningChef,
		char* lossingChef);


/**
 * Prints a message about the outcome of a competition between two chefs, in
 * which there's a tie between them.
 * @param output - File descriptor for the output channel.
 * @param chef1 - The name of a chef.
 * @param chef2 - The name of another chef.
 * Note: swapping the arguments for chef1 and chef2 will not change the
 * string this function prints to 'output', as this function uses the names of
 * the two chefs in the order determined by the lexicographic order of
 * the two names.
 */
void mtmPrintTieBetweenTwoChefs(FILE* output, char* chef1, char* chef2);


/**
 * Prints a message determining the leading chef.
 * @param output - File descriptor for the output channel.
 * @param name - The name of the leading chef.
 * @param goldPoints - the number of gold points the leading chef has.
 */
void mtmPrintLeadingChef(FILE* output, char* name, int goldPoints);


/**
 * Prints a message determining the top dish of a given chef (meaning: the first
 * dish the chef will use in the next contest against another chef).
 * @param output - File descriptor for the output channel.
 * @param chefName - The name of the chef.
 * @param dishName - The name of the chef's top dish.
 */
void mtmPrintTopDish(FILE* output, char* chefName, char* dishName);



/**
 * Prints a list of nicknames of all the current judges by the order they joined
 * the show.
 * @param output - File descriptor for the output channel.
 * @param judgeNicknames - An array of strings such that judgeNicknames[i] is
 * the nickname of the i-th judge (meaning: judgeNicknames[0] is the name of the
 * first judge to join the show among all current participating judges).
 * @param judgesNumber - the number of current participating judges (which is
 * also the number of elements in 'judgeNicknames').
 */
void mtmPrintAllJudges(FILE* output, char* judgeNicknames[], int judgesNumber);



#endif /* MTM_WET3_H_ */
