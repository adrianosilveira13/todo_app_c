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

/**
 * This function is really interesting to me because I'm starting to see how patterns of abstraction are
 * implemented in C. Here we have a function that loads tasks from a repository into the app structure.
 * The way the repository is implemented, whether if it's a file-based repository, an in-memory repository,
 * or a database-backed repository, is abstracted away from the app itself. The app just knows that it has
 * a repository that can load tasks. This is awesome because it allows for flexibility. I can use a file-based approach now
 * and later switch to a database-backed approach without changing the app logic.
 *
 */
int todo_app_load(TodoApp *app)
{
    if (!app->repo.load_all)
        return 0;                                          // if there is no load_all function in the repository contract, return failure
    return app->repo.load_all(app->repo.ctx, &app->tasks); // call the load_all function with the repository context and the task list to load tasks from the repository into the app
}

int todo_app_save(TodoApp *app)
{
    if (!app->repo.save_all)
        return 0;                                          // if there is no save_all function in the repository contract, return failure
    return app->repo.save_all(app->repo.ctx, &app->tasks); // call the save_all function with the repository context and the task list to save tasks into the repository
}

const TaskList *todo_app_tasks(const TodoApp *app)
{
    return &app->tasks; // return a pointer to the task list inside the app structure
}

int todo_app_add(TodoApp *app, const char *text)
{
    if (!text || text[0] == '\0')
        return 0; // validate the input text

    if (!task_list_push(&app->tasks, text, 0))
        return 0; // add the new task to the task list, if it fails it means it was a memory allocation failure

    return autosave(app); // after adding the task, automatically save the state of the app to the repository
}

int todo_app_edit(TodoApp *app, size_t index, const char *new_text)
{
    if (index >= app->tasks.count)
        return 0; // validate the index
    if (!new_text || new_text[0] == '\0')
        return 0;

    size_t n = strlen(new_text);        // allocates new memory for the new text
    char *copy = (char *)malloc(n + 1); // allocates memory for the new text plus the null terminator
    if (!copy)
        return 0;
    memcpy(copy, new_text, n + 1); // copy the new text into the allocated memory

    free(app->tasks.items[index].text);  // free the old text memory
    app->tasks.items[index].text = copy; // assign the new text to the task list

    return autosave(app); // after editing the task, auto save the state of the app to the repository
}

int todo_app_toggle(TodoApp *app, size_t index)
{
    if (index >= app->tasks.count)
        return 0; // validate the index

    app->tasks.items[index].completed = app->tasks.items[index].completed ? 0 : 1; // toggle the completed status of the task

    return autosave(app); // after toggling the task, auto save the state of the app to the repo
}

int todo_app_delete(TodoApp *app, size_t index)
{
    if (!task_list_remove_at(&app->tasks, index))
        return 0;         // remove the task at the specified index from the task list
    return autosave(app); // after deleting the task, auto save the state of the app to the repository
}