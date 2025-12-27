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

#endif