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
    int rc = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
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

