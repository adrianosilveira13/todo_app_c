#include "task.h"
#include <stdlib.h>
#include <string.h>

static char *xstrdup(const char *s)
{
    size_t n = strlen(s);            // this is the length of s without the terminator
    char *p = (char *)malloc(n + 1); // this is a new allocation for its copy including the space for the terminatior
    if (!p)                          // if allocation fails it will return null
        return NULL;
    memcpy(p, s, n + 1); // this one copies the incoming s string into the newly allocated space including the terminator
    return p;            // here I return the pointer to the duplicated string -> NOW I have the same string in two different memory locations
}

void task_list_init(TaskList *list) // this function initializes a TaskList structure by setting its items pointer to NULL and its count and capacity to 0.
{
    list->items = NULL; // Here I make sure that the items pointer is initialized to NULL. This indicates that there are no tasks currently stored in the list.
    list->count = 0;    // Here I set the count to 0, indicating that the list of items is currently empty.
    list->capacity = 0; // Here I set the capacity to 0, so there is no space allocated for items yet.
}

void task_list_free(TaskList *list) // This function exists to free the memory allocated and used by a TaskList structure.
{
    if (!list) // if the list pointer is null, there is nothing to free.
        return;
    for (size_t i = 0; i < list->count; i++) // this loop iterates over each task in the list, each task have a text field that is heap-allocated
    {
        free(list->items[i].text);  // Here I free the memory allocated for the text content of each task inside the list to prevent memory leaks.
        list->items[i].text = NULL; // After freeing the text, I set the pointer to NULL to avoid dangling pointers.
    }
    free(list->items);  // After freeing all the individual tasks contents, I free the memory allocated for the array of tasks inside the task list itself.
    list->items = NULL; // After freeing the items array, I set the pointer to NULL to avoid dangling pointers.
    list->count = 0;    // I reset the count to 0, indicating that there are no tasks left in the list.
    list->capacity = 0; // I set the capacity to 0, indicating that there is no allocated space for tasks anymore.
}

static int ensure_capacity(TaskList *list, size_t needed) // static function only visible within this file, this feels like a private method when i think about OOP
{
    if (list->capacity >= needed)
        return 1;                                                    // if the current capacity is already sufficient, return success
    size_t new_cap = (list->capacity == 0) ? 8 : list->capacity * 2; // if there is no buffer the capacity starts with 8, if there is already some buffer i double it

    while (new_cap < needed)
        new_cap *= 2; // keep doubling the capacity until it meets or exceeds the needed capacity

    Task *p = (Task *)realloc(list->items, new_cap * sizeof(Task)); // try to reallocate memory for the items array with the new capacity, by picking the array of tasks inside the items field of the TaskList structure
    if (!p)
        return 0; // if reallocation fails, return failure

    list->items = p;          // update the items pointer to the newly allocated memory
    list->capacity = new_cap; // update the capacity to the new capacity
    return 1;                 // return success
}

int task_list_push(TaskList *list, const char *text, int completed)
{
    if (!ensure_capacity(list, list->count + 1))
        return 0; // ensure there is enough capacity for one more task, if not return failure
    /**
     * I had some doubts about this part, so I asked GPT for clarification. My question was:
     * "Why is it necessary to duplicate the text string when adding a new task to the task list? Isn't it enough to just assign the pointer?"
     *
     * And this was the answer I got:
     *
     * Creates a duplicate of the given text string.
     *
     * This is necessary to ensure that the task structure owns its own copy of the text,
     * preventing issues if the original string is modified or freed elsewhere.
     * By duplicating the string, each task maintains its own independent copy,
     * avoiding unintended side effects and potential bugs.
     *
     * Although this approach uses additional memory, it is a common and safe practice
     * in C to manage string ownership and lifetime, especially when storing strings
     * in data structures like lists.
     *
     * If 'text' is NULL, an empty string is duplicated instead.
     */
    char *copy = xstrdup(text ? text : ""); // this line calls the private helper function xstrdup to create a duplicate of the input text string
    if (!copy)
        return 0; // if duplication fails, return failure

    list->items[list->count].text = copy;                   // assing the copy of the incoming text string to the text field of the new task being added to the list, at the index indicated by count
    list->items[list->count].completed = completed ? 1 : 0; // assign the completed status to the new task being added to the list
    return 1;                                               // return success
}

int task_list_remove_at(TaskList *list, size_t index)
{
    if (index >= list->count)
        return 0; // if the index is out of bounds, return failure

    free(list->items[index].text); // free the memory allocated for the text of the task being removed to prevent memory leaks

    for (size_t i = index; i + 1 < list->count; i++) // while there are still items (i + 1) inside the list after the index
    {
        list->items[i] = list->items[i + 1]; // shift each subsequent task one position to the left to fill the gap left by the removed task
    }

    list->count--; // decrement the count of tasks in the list
    return 1;      // return success
}