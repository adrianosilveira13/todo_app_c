#ifndef TASK_H
#define TASK_H

#include <stddef.h>

typedef struct
{
    char *text;    // heap-allocated string
    int completed; // 0 for incomplete, 1 for completed
} Task;

typedef struct
{
    Task *tasks;
    size_t count;
    size_t capacity;
} TaskList;

// lifecycle
void task_list_init(TaskList *list);
void task_list_free(TaskList *list);

// operations
int task_list_push(TaskList *list, const char *text, int completed);
int task_list_remove_at(TaskList *list, size_t index);

#endif // TASK_H