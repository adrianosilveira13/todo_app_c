#ifndef TASK_REPO_SQLITE_H
#define TASK_REPO_SQLITE_H

#include "../app/todo_app.h"

TaskRepository task_repo_sqlite_create(const char *db_path);

#endif