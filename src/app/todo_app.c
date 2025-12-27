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

void todo_app_free(TodoApp *app)
{
    task_list_free(&app->tasks); // free the memory allocated for the task list inside the app structure

    if (app->repo.destroy)
    {
        app->repo.destroy(app->repo.ctx); // if there is a destroy function in the repository contract, call it to clean up any resources associated with the repository context
    }

    app->repo.ctx = NULL; // set the repo context pointer to NULL to avoid dangling pointers
    app->repo.load_all = NULL;
    app->repo.save_all = NULL;
    app->repo.destroy = NULL;
}