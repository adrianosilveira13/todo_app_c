#include "task.h"
#include <stdlib.h>
#include <string.h>

static char *xstrdup(const char *s)
{
    size_t n = strlen(s);            // this is the length of s without the terminator
    char *p = (char *)malloc(n + 1); // this is a new allocation for its copy including the space for the terminatior
    if (!p)                          // if allocation fails it will return null
        return NULL;
    memcpy(p, s, n + 1); // this one copies the incoming s string into the newly allocated space including the terminator
    return p;            // here I return the pointer to the duplicated string -> NOW I have the same string in two different memory locations
}