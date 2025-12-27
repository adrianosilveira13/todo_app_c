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
