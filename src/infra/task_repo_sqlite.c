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

