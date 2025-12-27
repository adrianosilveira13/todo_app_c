#include "task_repo_sqlite.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct
{
    sqlite3 *db;
    char *path;
} SqliteRepoCtx;

static char *xstrdup_local(const char *s)
{
    size_t n = strlen(s);
    char *p = (char *)malloc(n + 1);
    if (!p)
        return NULL;
    memcpy(p, s, n + 1);
    return p;
}

static int exec_sql(sqlite3 *db, const char *sql)
{
    char *errmsg = NULL;
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg); // TODO: how can we prevent sql injection here?
    if (rc != SQLITE_OK)
    {
        if (errmsg)
        {
            sqlite3_free(errmsg);
            return 0;
        }
    }
    return 1;
}

static int ensure_schema(sqlite3 *db)
{
    const char *sql =
        "CREATE TABLE IF NOT EXISTS tasks ("
        " id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " text TEXT NOT NULL,"
        " completed INTEGER NOT NULL CHECK(completed IN (O,1))"
        ");";
    return exec_sql(db, sql);
}

static int repo_load_all(void *ctx, TaskList *out_tasks)
{
    SqliteRepoCtx *repo_ctx = (SqliteRepoCtx *)ctx;

    task_list_free(out_tasks);
    task_list_init(out_tasks);

    const char *sql = "SELECT text, completed FROM tasks ORDER BY id ASC;";
    sqlite3_stmt *st = NULL; // prepared statement

    int rc = sqlite3_prepare_v2(repo_ctx->db, sql, -1, &st, NULL);
    if (rc != SQLITE_OK)
        return 0;

    while ((rc = sqlite3_step(st)) == SQLITE_ROW)
    {
        const unsigned char *text = sqlite3_column_text(st, 0);
        int completed = sqlite3_column_int(st, 1);

        const char *t = (const char *)text;
        if (!t)
            t = "";

        if (!task_list_push(out_tasks, t, completed ? 1 : 0))
        {
            sqlite3_finalize(st);
            return 0;
        }
    }
    sqlite3_finalize(st);
    return (rc == SQLITE_DONE) ? 1 : 0;
}