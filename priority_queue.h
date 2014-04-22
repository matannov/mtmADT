#ifndef PRIORITY_QUEUE_H_
#define PRIORITY_QUEUE_H_

#include <stdbool.h>
/**
 * Priority Queue Container
 *
 * Implements a priority queue container type.
 * Each element added to the container has a constant integer priority.
 * The elements held by the container are always ordered by their priority.
 * The "top element" is the element with highest priority, and if there are more
 * than one then it's the first that was added to the priority queue among them.
 *
 * The following functions are available:
 *
 *   priorityQueueCreate      - Creates a new empty priority queue
 *   priorityQueueDestroy     - Deletes an existing priority queue and frees
 *   							all resources.
 *   priorityQueuCopy    	  - Copies an existing priority queue.
 *   priorityQueueGetSize     - Returns the size of a given priority queue.
 *   priorityQueuTop      	  - Return the top element.
 *   priorityQueueAdd         - Adds an element into the priority queue.
 *   priorityQueuePop		  - Removes the top element.
 */

/** Type for defining the priority queue */
typedef struct PriorityQueue_t *PriorityQueue;

/** Type used for returning error codes from priority queue functions
 * PRIORITY_QUEUE_NULL_ARGUMENT may be returned from functions without
 * mentioning it in the documentation. */
typedef enum PriorityQueueResult {
	PRIORITY_QUEUE_SUCCESS,
	PRIORITY_QUEUE_NULL_ARGUMENT,
	PRIORITY_QUEUE_OUT_OF_MEMORY,
	PRIORITY_QUEUE_EMPTY
} PriorityQueueResult;


/** Type representing the priority of an element */
typedef int Priority;


/** Element data type for priority queue container */
typedef void* PriorityQueueElement;

/**
 * Type of function for copying an element of the priority queue.
 *
 * Such a function should be supplied to a priority queue to allow it to copy its
 * elements. The function supplied should be able to dynamically copy the
 * object and return a pointer to a newly allocated object.
 * If this function fails for some reason, it should return NULL.
 * IMPORTANT: The function must not return NULL unless it fails.
 *
 * For example, here is a proper function for copying a string that can be
 * used for storing strings in a priority queue:
 * @code
 * PriorityQueueElement copyString(PriorityQueueElement str) {
 *   assert(str);
 *   char* copy = malloc(strlen(str) + 1);
 *   return copy ? strcpy(copy, str) : NULL;
 * }
 * @endcode
 */
typedef PriorityQueueElement (*CopyPriorityQueueElement)(PriorityQueueElement);

/** Type of function for deallocating an element of the priorityQueue */
typedef void (*FreePriorityQueueElement)(PriorityQueueElement);



/**
 * Allocates a new PriorityQueue.
 *
 * Creates a new empty priority queue. This function receives the functions which will be
 * used for copying elements into the priority queue and freeing them when needed.
 * For example, in order to create a priority queue of strings one need to define these
 * two functions:
 * @code
 * PriorityQueueElement copyString(PriorityQueueElement str);
 * void freeString(PriorityQueueElement str);
 * @endcode
 * And then create a priorityQueue as follows:
 * @code
 * PriorityQueue priorityQueueOfStrings = priorityQueueCreate(copyString, freeString);
 * @endcode
 *
 * @param copyElement Function pointer to be used for copying elements into
 * the priorityQueue or when copying the priorityQueue.
 * IMPORTANT: The copyElement function must not return NULL unless it fails.
 * @param freeElement Function pointer to be used for removing elements from
 * the priorityQueue.
 * @return
 * 	NULL - if one of the parameters is NULL or allocations failed.
 * 	A new PriorityQueue in case of success.
 */
PriorityQueue priorityQueueCreate(CopyPriorityQueueElement copyElement,
		FreePriorityQueueElement freeElement);



/**
 * priorityQueueDestroy: Deallocates an existing priorityQueue. Clears all
 * elements by using the stored free function.
 *
 * @param priorityQueue Target priorityQueue to be deallocated. If priorityQueue
 * is NULL nothing will be done.
 */
void priorityQueueDestroy(PriorityQueue priorityQueue);



/**
 * Creates a copy of target priorityQueue.
 *
 * The new copy will contain all the elements from the source priorityQueue in the same
 * order and will use the same functions as the original priorityQueue for copying and
 * freeing elements.
 *
 * @param priorityQueue The target priorityQueue to copy
 * @return
 * NULL if a NULL was sent or a memory allocation failed.
 * A PriorityQueue containing the same elements with same order as priorityQueue otherwise.
 */
PriorityQueue priorityQueueCopy(PriorityQueue priorityQueue);

/**
 * Returns the number of elements in a priorityQueue
 *
 * @param priorityQueue The target priorityQueue which size is requested.
 * @return
 * -1 if a NULL pointer was sent.
 * Otherwise the number of elements in the priorityQueue.
 */
int priorityQueueGetSize(PriorityQueue priorityQueue);



/**
 * Retrieves the top element (see definition at the top of this file).
 *
 * @param priorityQueue The target priorityQueue.
 * @return
 * The element that would have been removed if priorityQueuePop was called.
 * Note that the actual element is returned (and not a copy of the element).
 * If the priority queue is empty, NULL is returned.
 */
PriorityQueueElement priorityQueueTop(PriorityQueue priorityQueue);


/**
 * Adds a new element to the priority queue.
 *
 * @param priorityQueue The target priorityQueue.
 * @param element The element to add. A copy of the element will be
 * added using the supplied copying function.
 * @param priority The priority of the new element.
 * @return
 * PRIORITY_QUEUE_OUT_OF_MEMORY if out of memory.
 * PRIORITY_QUEUE_SUCCESS if successful.
 */
PriorityQueueResult priorityQueueAdd(PriorityQueue priorityQueue,
		PriorityQueueElement element, Priority priority);


/**
 * Removes the top element (see definition at the top of this file) from the
 * priority queue.
 * Note: this function releases the element using the supplied
 * FreePriorityQueueElement function.
 *
 * @param priorityQueue The target priorityQueue.
 * @return
 * PRIORITY_QUEUE_EMPTY if the priority queue is empty.
 * PRIORITY_QUEUE_SUCCESS if successful.
 */
PriorityQueueResult priorityQueuePop(PriorityQueue priorityQueue);


#endif /* PRIORITY_QUEUE_H_ */
