#include "infra/task_repo_sqlite.h"
#include "app/todo_app.h"
#include "ui/cli/cli.h"
#include <stdio.h>

int main(void)
{
  TaskRepository repo = task_repo_sqlite_create("tasks.db");
  if (!repo.ctx)
  {
    printf("Falha ao inicializar o repositório.\n");
    return 1;
  }

  TodoApp app;
  todo_app_init(&app, repo);

  if (!todo_app_load(&app))
  {
    printf("Aviso: Falha ao carregar. Iniciando vazio.\n");
  }

  cli_run(&app);

  todo_app_free(&app);
  return 0;
}