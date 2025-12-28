#include "text.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *text_read_line(const char *prompt)
{
    if (prompt)
    {
        printf("%s", prompt);
        fflush(stdout);
    }

    size_t cap = 64, len = 0;
    char *buffer = (char *)malloc(cap);
    if (!buffer)
        return NULL;

    int c;
    while ((c = getchar()) != EOF)
    {
        if (c == '\n')
            break;
        if (len + 1 >= cap)
        {
            cap *= 2;
            char *p = (char *)realloc(buffer, cap);
            if (!p)
            {
                free(buffer);
                return NULL;
            }
            buffer = p;
        }
        buffer[len++] = (char)c;
    }

    buffer[len] = '\0';
    return buffer;
}

int text_contains_case_insensitive(const char *haystack, const char *needle)
{
    if (!needle || needle[0] == '\0')
        return 1; // An empty needle is always found

    size_t h = strlen(haystack);
    size_t n = strlen(needle);
    if (n > h)
        return 0; // Needle longer than haystack cannot be found

    for (size_t i = 0; i + n <= h; i++)
    {
        size_t j = 0;
        for (; j < n; j++)
        {
            unsigned char a = (unsigned char)haystack[i + j];
            unsigned char b = (unsigned char)needle[j];
            if (tolower(a) != tolower(b))
                break;
        }
        if (j == n)
            return 1; // Found a match
    }
    return 0; // No match found
}