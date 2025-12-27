#ifndef TODO_APP_H
#define TODO_APP_H

#include "../domain/task.h"
#include <stddef.h>

typedef struct TaskRepository
{
    void *ctx;                                      // this is a generic pointer to any context data needed by the repository implementation
    int (*load_all)(void *ctx, TaskList *out);      // this is a pointer to a function called load_all that receives a context pointer and a pointer to a TaskList. It returns an integer
    int (*save_all)(void *ctx, const TaskList *in); // this is a pointer to a function called save_all that receives a context pointer and a pointer to a constant TaskList. It returns an integer
    void (*destroy)(void *ctx);                     // this is a pointer to a function called destroy that receives a context pointer and returns nothing (void), I think this one will be used to clean up any resources associated with the repository context
} TaskRepository;

typedef struct TodoApp
{
    TaskRepository repo;
    TaskList tasks;
} TodoApp;

typedef enum
{
    SORT_TEXT_AZ = 1,
    SORT_TEXT_Z1 = 2,
    SORT_PENDING_FIRST = 3,
    SORT_DONE_FIRST = 4,
} SortMode;

/* lifecycle management functions for TodoApp */
void todo_app_init(TodoApp *app, TaskRepository repo);
void todo_app_free(TodoApp *app);

/* persistency */
int todo_app_load(TodoApp *app);
int todo_app_save(TodoApp *app);

/** reading (view) */
const TaskList *todo_app_tasks(const TodoApp *app);

/** CRUD */
int todo_app_add(TodoApp *app, const char *text);
int todo_app_edit(TodoApp *app, size_t index, const char *new_text);
int todo_app_toggle(TodoApp *app, size_t index);
int todo_app_delete(TodoApp *app, size_t index);

/* sorting */
int todo_app_sort(TodoApp *app, SortMode mode);

/**
 * Searching:
 * - Search query as a substring case-insensitive.
 * - Returns an array of indices (0-based) allocated on the heap.
 * - Caller needs to free the (*out_indices) array when it is no longer needed.
 *
 * Output contract:
 * - If success: returns 1
 * -- *out_count can be 0 (no matches found)
 * -- *out_indices will be NULL if out_count is 0
 * - If failure: returns 0
 *
 */

int todo_app_search(const TodoApp *app, const char *query, size_t **out_indices, size_t *out_count);

#endif