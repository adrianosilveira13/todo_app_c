#include "todo_app.h"

#include <stdlib.h>
#include <string.h>

static int autosave(TodoApp *app) // this function ensures that any operation that modifies the state of the task persist at the repository
{
    return todo_app_save(app);
}

void todo_app_init(TodoApp *app, TaskRepository repo)
{
    app->repo = repo;            // copy the contract of the repository into the app structure
    task_list_init(&app->tasks); // initialize the task list inside the app structure
}