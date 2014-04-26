#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "test_utilities.h"
#include "../list.h"

/**
 * We're going to be doing this a lot, so we define a macro for this specific
 * test. You can define many such macros for repetitive assertions in your
 * unit-tests.
 */
#define ASSERT_SAME_PERSON(a,b) ASSERT_TEST(personCompare(a,b) == 0)

/** A simple datatype of Person to be used in testing the list
 * Note that this is not an ADT because the struct is exposed.
 **/
typedef struct Person_t {
	char* name;
	int id;
} *Person;

static Person personCreate(char* name, int id);
static Person personCopy(Person person);
static void personDestroy(Person person);
static int personCompare(Person a, Person b);
static char* stringDuplicate(char* str);

static char* stringDuplicate(char* str) {
	assert(str);
	char* result = malloc(strlen(str)+1);
	return result ? strcpy(result,str) : NULL;
}

static void personDestroy(Person person) {
	if (!person) {
		return;
	}
	free(person->name);
	free(person);
}

static Person personCreate(char* name, int id) {
	assert(name);
	Person person = malloc(sizeof(struct Person_t));
	if (!person) {
		return NULL;
	}
	person->name = stringDuplicate(name);
	if (!name) {
		personDestroy(person);
		return NULL;
	}
	person->id = id;
	return person;
}

static Person personCopy(Person person) {
	assert(person);
	return personCreate(person->name, person->id);
}

static int personCompare(Person a, Person b) {
	assert(a && b);
	int namesDifference = strcmp(a->name, b->name);
	if (namesDifference != 0) {
		return namesDifference;
	}
	return a->id - b->id;
}

/*!
 * Helper functions for storing Person objects in a list
 */
static ListElement personCopyHelper(ListElement element) {
	return personCopy(element);
}

static void personDestroyHelper(ListElement element) {
	personDestroy(element);
}

static int personCompareHelper(ListElement element1, ListElement element2) {
	return personCompare(element1, element2);
}


/**
 * This struct is used to hold a set of examples
 * This allows us to save code on reinitializing big examples in the start of
 * each test
 */
typedef struct {
	Person haimMoshe;
	Person oferLevi;
	Person eyalGolan;
	Person aviBitter;
	Person idanTal;
	List list;
} Examples;

/**
 * This function is called by the SET_UP macro in testing_utilities. This
 * function should initialize an examples struct that can be used for testing
 * the ADT
 * @return a new fully initialize examples sturct
 */
static Examples setUp() {
	Examples examples;
	examples.haimMoshe = personCreate("Haim Moshe", 111111111);
	examples.oferLevi = personCreate("Ofer Levi", 222222222);
	examples.eyalGolan = personCreate("Eyal Golan", 333333333);
	examples.aviBitter = personCreate("Avi Bitter", 444444444);
	examples.idanTal = personCreate("Idan Tal", 555555555);
	examples.list = listCreate(personCopyHelper, personDestroyHelper);
	listInsertLast(examples.list, examples.eyalGolan);
	listInsertLast(examples.list, examples.oferLevi);
	listInsertLast(examples.list, examples.aviBitter);
	listInsertLast(examples.list, examples.haimMoshe);
	return examples;
}

/**
 * Called by the TEAR_DOWN macro from testing_utilities.h. This function should
 * deallocate and clear all the code
 * @param examples
 */
void tearDown(Examples examples) {
	personDestroy(examples.haimMoshe);
	personDestroy(examples.oferLevi);
	personDestroy(examples.eyalGolan);
	personDestroy(examples.aviBitter);
	personDestroy(examples.idanTal);
	listDestroy(examples.list);
}

/**
 * List of tests, one for each function is usually a good thumb rule.
 */
static bool testListCreate() {
	ASSERT_TEST(listCreate(NULL,NULL) == NULL);
	ASSERT_TEST(listCreate(NULL,NULL) == NULL);
	ASSERT_TEST(listCreate(personCopyHelper,NULL) == NULL);
	ASSERT_TEST(listCreate(NULL,personDestroyHelper) == NULL);
	List list = listCreate(personCopyHelper, personDestroyHelper);
	ASSERT_TEST(list != NULL);
	listDestroy(list);

	return true;
}

static bool testListCopy() {
	SET_UP(Examples);
	ASSERT_TEST(listCopy(NULL) == NULL);
	List list = listCreate(personCopyHelper, personDestroyHelper);
	List copy = listCopy(list);
	ASSERT_TEST(copy != NULL);
	ASSERT_TEST(0 == listGetSize(copy));
	listInsertFirst(list, examples.aviBitter);
	ASSERT_TEST(0 == listGetSize(copy));

	List copy2 = listCopy(examples.list);
	ASSERT_TEST(4 == listGetSize(copy2));
	ASSERT_SAME_PERSON(examples.eyalGolan, listGetFirst(copy2));
	ASSERT_SAME_PERSON(examples.oferLevi, listGetNext(copy2));
	ASSERT_SAME_PERSON(examples.aviBitter, listGetNext(copy2));
	ASSERT_SAME_PERSON(examples.haimMoshe, listGetNext(copy2));
	ASSERT_TEST(listGetNext(copy2) == NULL);

	listDestroy(list);
	listDestroy(copy);
	listDestroy(copy2);
	TEAR_DOWN();

	return true;
}

static bool testListGetSize() {
	SET_UP(Examples);
	List list = listCreate(personCopyHelper, personDestroyHelper);
	ASSERT_TEST(0 == listGetSize(list));
	listInsertFirst(list, examples.aviBitter);
	ASSERT_TEST(1 == listGetSize(list));
	listInsertFirst(list, examples.aviBitter);
	ASSERT_TEST(2 == listGetSize(list));
	listGetFirst(list);
	listRemoveCurrent(list);
	ASSERT_TEST(1 == listGetSize(list));
	listDestroy(list);
	TEAR_DOWN();
	return true;
}

static bool testListGetFirst() {
	SET_UP(Examples);
	List list = listCreate(personCopyHelper, personDestroyHelper);
	ASSERT_TEST(listGetFirst(list) == NULL);
	Person first = listGetFirst(examples.list);
	ASSERT_SAME_PERSON(examples.eyalGolan, first);
	ASSERT_SAME_PERSON(first, listGetFirst(examples.list));
	listDestroy(list);
	TEAR_DOWN();
	return true;
}

static bool testListGetNext() {
	SET_UP(Examples);
	ASSERT_TEST(listGetNext(NULL) == NULL);
	ASSERT_TEST(listGetNext(examples.list) == NULL);
	TEAR_DOWN();
	return true;
}

static bool testListInsertFirst() {
	SET_UP(Examples);
	ASSERT_TEST(LIST_NULL_ARGUMENT == listInsertFirst(NULL, NULL));
	ASSERT_TEST(LIST_SUCCESS ==
			listInsertFirst(examples.list, examples.idanTal));
	ASSERT_TEST(5 == listGetSize(examples.list));
	ASSERT_SAME_PERSON(examples.idanTal,listGetFirst(examples.list));
	ASSERT_SAME_PERSON(examples.eyalGolan,listGetNext(examples.list));
	ASSERT_SAME_PERSON(examples.oferLevi,listGetNext(examples.list));
	ASSERT_SAME_PERSON(examples.aviBitter,listGetNext(examples.list));
	ASSERT_SAME_PERSON(examples.haimMoshe,listGetNext(examples.list));
	ASSERT_TEST(listGetNext(examples.list) == NULL);
	TEAR_DOWN();
	return true;
}

static bool testListInsertLast() {
	SET_UP(Examples);
	ASSERT_TEST(LIST_NULL_ARGUMENT == listInsertLast(NULL, NULL));
	ASSERT_TEST(LIST_SUCCESS ==
			listInsertLast(examples.list, examples.idanTal));
	ASSERT_TEST(5 == listGetSize(examples.list));
	ASSERT_SAME_PERSON(examples.eyalGolan,listGetFirst(examples.list));
	ASSERT_SAME_PERSON(examples.oferLevi,listGetNext(examples.list));
	ASSERT_SAME_PERSON(examples.aviBitter,listGetNext(examples.list));
	ASSERT_SAME_PERSON(examples.haimMoshe,listGetNext(examples.list));
	ASSERT_SAME_PERSON(examples.idanTal,listGetNext(examples.list));
	ASSERT_TEST(listGetNext(examples.list) == NULL);
	TEAR_DOWN();
	return true;
}

static bool testListInsertBeforeCurrent() {
	SET_UP(Examples);
	ASSERT_TEST(LIST_NULL_ARGUMENT == listInsertBeforeCurrent(NULL, NULL));
	ASSERT_TEST(LIST_INVALID_CURRENT ==
			listInsertBeforeCurrent(examples.list,NULL));
	listGetFirst(examples.list);
	listGetNext(examples.list);
	ASSERT_TEST(LIST_SUCCESS ==
			listInsertBeforeCurrent(examples.list, examples.idanTal));

	ASSERT_TEST(5 == listGetSize(examples.list));
	ASSERT_SAME_PERSON(examples.eyalGolan,listGetFirst(examples.list));
	ASSERT_SAME_PERSON(examples.idanTal,listGetNext(examples.list));
	ASSERT_SAME_PERSON(examples.oferLevi,listGetNext(examples.list));
	ASSERT_SAME_PERSON(examples.aviBitter,listGetNext(examples.list));
	ASSERT_SAME_PERSON(examples.haimMoshe,listGetNext(examples.list));
	ASSERT_TEST(listGetNext(examples.list) == NULL);
	TEAR_DOWN();
	return true;
}

static bool testListInsertAfterCurrent() {
	SET_UP(Examples);
	ASSERT_TEST(LIST_NULL_ARGUMENT == listInsertAfterCurrent(NULL, NULL));
	ASSERT_TEST(LIST_INVALID_CURRENT ==
			listInsertAfterCurrent(examples.list, NULL));
	listGetFirst(examples.list);
	listGetNext(examples.list);
	ASSERT_TEST(LIST_SUCCESS ==
			listInsertAfterCurrent(examples.list, examples.idanTal));

	ASSERT_TEST(5 == listGetSize(examples.list));
	ASSERT_SAME_PERSON(examples.eyalGolan,listGetFirst(examples.list));
	ASSERT_SAME_PERSON(examples.oferLevi,listGetNext(examples.list));
	ASSERT_SAME_PERSON(examples.idanTal,listGetNext(examples.list));
	ASSERT_SAME_PERSON(examples.aviBitter,listGetNext(examples.list));
	ASSERT_SAME_PERSON(examples.haimMoshe,listGetNext(examples.list));
	ASSERT_TEST(listGetNext(examples.list) == NULL);
	TEAR_DOWN();
	return true;
}

static bool testListRemoveCurrent() {
	SET_UP(Examples);
	ASSERT_TEST(LIST_NULL_ARGUMENT == listRemoveCurrent(NULL));
	ASSERT_TEST(LIST_INVALID_CURRENT == listRemoveCurrent(examples.list));

	listGetFirst(examples.list);
	listGetNext(examples.list);
	listRemoveCurrent(examples.list);
	ASSERT_TEST(3 == listGetSize(examples.list));
	ASSERT_SAME_PERSON(examples.eyalGolan,listGetFirst(examples.list));
	ASSERT_SAME_PERSON(examples.aviBitter,listGetNext(examples.list));
	ASSERT_SAME_PERSON(examples.haimMoshe,listGetNext(examples.list));
	ASSERT_TEST(listGetNext(examples.list) == NULL);

	ASSERT_TEST(LIST_INVALID_CURRENT == listRemoveCurrent(examples.list));
	TEAR_DOWN();
	return true;
}

static bool testListSort() {
	SET_UP(Examples);
	ASSERT_TEST(LIST_NULL_ARGUMENT == listSort(NULL,NULL));
	List list = listCreate(personCopyHelper, personDestroyHelper);
	ASSERT_TEST(LIST_NULL_ARGUMENT == listSort(list,NULL));
	ASSERT_TEST(LIST_NULL_ARGUMENT == listSort(NULL,personCompareHelper));
	listDestroy(list);

	listSort(examples.list, personCompareHelper);
	ASSERT_TEST(4 == listGetSize(examples.list));
	ASSERT_SAME_PERSON(examples.aviBitter,listGetFirst(examples.list));
	ASSERT_SAME_PERSON(examples.eyalGolan,listGetNext(examples.list));
	ASSERT_SAME_PERSON(examples.haimMoshe,listGetNext(examples.list));
	ASSERT_SAME_PERSON(examples.oferLevi,listGetNext(examples.list));
	ASSERT_TEST(listGetNext(examples.list) == NULL);
	TEAR_DOWN();
	return true;
}

static bool testListClear() {
	SET_UP(Examples);
	ASSERT_TEST(listClear(examples.list) == LIST_SUCCESS);
	ASSERT_TEST(0 == listGetSize(examples.list));
	List list = listCreate(personCopyHelper, personDestroyHelper);
	listClear(list);
	ASSERT_TEST(0 == listGetSize(list));
	listDestroy(list);
	TEAR_DOWN();
	return true;
}

static bool testListDestroy() {
	listDestroy(NULL);
	return true;
}

int main() {
	RUN_TEST(testListCreate);
	RUN_TEST(testListCopy);
	RUN_TEST(testListGetSize);
	RUN_TEST(testListGetFirst);
	RUN_TEST(testListGetNext);
	RUN_TEST(testListInsertFirst);
	RUN_TEST(testListInsertLast);
	RUN_TEST(testListInsertBeforeCurrent);
	RUN_TEST(testListInsertAfterCurrent);
	RUN_TEST(testListRemoveCurrent);
	RUN_TEST(testListSort);
	RUN_TEST(testListClear);
	RUN_TEST(testListDestroy);
	return 0;
}
