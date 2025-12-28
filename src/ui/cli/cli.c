#include "cli.h"
#include "../../util/text.h"
#include <stdio.h>
#include <stdlib.h>

static void print_menu()
{
    printf("\n=== TODO (CLI) ===\n");
    printf("1) Listar tarefas\n");
    printf("2) Adicionar tarefa\n");
    printf("3) Editar tarefa\n");
    printf("4) Marcar/Desmarcar concluída\n");
    printf("5) Excluir tarefa\n");
    printf("6) Buscar\n");
    printf("7) Ordenar\n");
    printf("8) Recarregar do arquivo\n");
    printf("9) Salvar agora\n");
    printf("0) Sair\n");
}

static void list_tasks(TodoApp *app)
{
    const TaskList *t = todo_app_tasks(app);
    if (t->count == 0)
    {
        printf("Nenhuma tarefa encontrada.\n");
        return;
    }

    for (size_t i = 0; i < t->count; i++)
    {
        printf("%3zu) [%c] %s\n", i + 1, t->items[i].completed ? 'X' : ' ', t->items[i].text);
    }
}

static int pick_index_1based(const TodoApp *app, const char *prompt, size_t *out_index0)
{
    const TaskList *t = todo_app_tasks(app);
    if (t->count == 0)
        return 0;

    int n;
    if (!text_read_int_range(prompt, 1, (int)t->count, &n))
        return 0;
    *out_index0 = (size_t)(n - 1);
    return 1;
}

static void add_flow(TodoApp *app)
{
    char *text = text_read_line("Digite a nova tarefa: ");
    if (!text)
        return;

    if (text[0] == '\0')
    {
        printf("Tarefa vazia não será adicionada.\n");
        free(text);
        return;
    }

    if (!todo_app_add(app, text))
    {
        printf("Falha ao adicionar a tarefa.\n");
    }
    else
    {
        printf("Tarefa adicionada com sucesso.\n");
    }

    free(text);
}

static void edit_flow(TodoApp *app)
{
    list_tasks(app);
    size_t idx;
    if (!pick_index_1based(app, "Número da tarefa para editar: ", &idx))
    {
        printf("Cancelado.\n");
        return;
    }

    const TaskList *t = todo_app_tasks(app);
    char prompt[256];
    snprintf(prompt, sizeof(prompt), "Novo texto (antes: \"%s\"): ", t->items[idx].text);

    char *new_text = text_read_line(prompt);
    if (!new_text)
        return;

    if (new_text[0] == '\0')
    {
        printf("Texto vazio. Edição cancelada.\n");
        free(new_text);
        return;
    }

    if (!todo_app_edit(app, idx, new_text))
    {
        printf("Falha ao editar a tarefa.\n");
    }
    else
    {
        printf("Tarefa editada com sucesso.\n");
    }
    free(new_text);
}

static void toggle_flow(TodoApp *app)
{
    list_tasks(app);
    size_t idx;
    if (!pick_index_1based(app, "Numéro da tarefa para marcar/desmarcar: ", &idx))
    {
        printf("Cancelado.\n");
        return;
    }

    if (!todo_app_toggle(app, idx))
    {
        printf("Falha ao atualizar a tarefa.\n");
    }
    else
    {
        printf("Tarefa atualizada com sucesso.\n");
    }
}

static void delete_flow(TodoApp *app)
{
    list_tasks(app);
    size_t idx;
    if (!pick_index_1based(app, "Número da tarefa para excluir: ", &idx))
    {
        printf("Cancelado.\n");
        return;
    }

    if (!todo_app_delete(app, idx))
    {
        printf("Falha ao excluir a tarefa.\n");
    }
    else
    {
        printf("Tarefa excluída com sucesso.\n");
    }
}

static void search_flow(TodoApp *app)
{
    char *q = text_read_line("Buscar por (texto): ");
    if (!q)
        return;

    size_t *idx = NULL, n = 0;
    if (!todo_app_search(app, q, &idx, &n))
    {
        printf("Falha na busca.\n");
        free(q);
        return;
    }

    if (n == 0)
    {
        printf("Nenhuma tarefa encontrada para \"%s\".\n", q);
        free(q);
        return;
    }

    const TaskList *t = todo_app_tasks(app);
    for (size_t i = 0; i < n; i++)
    {
        size_t k = idx[i];
        printf("%3zu) [%c] %s\n", k + 1, t->items[k].completed ? 'X' : ' ', t->items[k].text);
    }

    free(idx);
    free(q);
}

static void sort_flow(TodoApp *app)
{
    printf("Ordenar por:\n");
    printf("  1) Texto (A -> Z)\n");
    printf("  2) Texto (Z -> A)\n");
    printf("  3) Status (pendentes primeiro)\n");
    printf("  4) Status (concluídas primeiro)\n");

    int opt;
    if (!text_read_int_range("Escolha uma opção: ", 1, 4, &opt))
    {
        printf("Cancelado.\n");
        return;
    }

    if (!todo_app_sort(app, (SortMode)opt))
    {
        printf("Falha ao ordenar as tarefas.\n");
    }
    else
    {
        printf("Tarefas ordenadas com sucesso.\n");
    }
}

void cli_run(TodoApp *app)
{
    for (;;)
    {
        print_menu();

        int opt;
        if (!text_read_int_range("Escolha uma opção: ", 0, 9, &opt))
        {
            continue;
        }

        switch (opt)
        {
        case 1:
            list_tasks(app);
            break;
        case 2:
            add_flow(app);
            break;
        case 3:
            edit_flow(app);
            break;
        case 4:
            toggle_flow(app);
            break;
        case 5:
            delete_flow(app);
            break;
        case 6:
            search_flow(app);
            break;
        case 7:
            sort_flow(app);
            break;
        case 8:
            if (!todo_app_load(app))
                printf("Falha ao recarregar.\n");
            else
                printf("Recarregado.\n");
            break;
        case 9:
            if (!todo_app_save(app))
                printf("Falha ao salvar.\n");
            else
                printf("Salvo.\n");
            break;
        case 0:
            // tenta salvar por segurança
            todo_app_save(app);
            return;
        }
    }
}