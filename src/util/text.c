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

int text_read_int_range(const char *prompt, int min, int max, int *out)
{
    for (;;)
    {
        char *line = text_read_line(prompt);
        if (!line)
            return 0;

        if (line[0] == '\0')
        {
            free(line);
            return 0;
        }

        char *end = NULL;
        long v = strtol(line, &end, 10);

        if (end && *end == '\0' && v >= min && v <= max)
        {
            *out = (int)v;
            free(line);
            return 1;
        }

        printf("Entrada inválida. Digite um número entre %d e %d.\n", min, max);
        free(line);
    }
}

char *text_encode_one_line(const char *raw)
{
    size_t n = 0;
    for (size_t i = 0; raw[i]; i++)
    {
        switch (raw[i])
        {
        case '\\':
        case '\n':
        case '\t':
        case '\r':
            n += 2; // Escape sequence
            break;
        default:
            n += 1;
            break;
        }
    }

    char *enc = (char *)malloc(n + 1);
    if (!enc)
        return NULL;

    size_t j = 0;
    for (size_t i = 0; raw[i]; i++)
    {
        switch (raw[i])
        {
        case '\\':
            enc[j++] = '\\';
            enc[j++] = '\\';
            break;
        case '\n':
            enc[j++] = '\\';
            enc[j++] = 'n';
            break;
        case '\t':
            enc[j++] = '\\';
            enc[j++] = 't';
            break;
        case '\r':
            enc[j++] = '\\';
            enc[j++] = 'r';
            break;
        default:
            enc[j++] = raw[i];
            break;
        }
    }
    enc[j] = '\0';
    return enc;
}

char *text_decode_one_line(const char *enc)
{
    size_t len = strlen(enc);
    char *raw = (char *)malloc(len + 1);
    if (!raw)
        return NULL;

    size_t j = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (enc[i] == '\\' && i + 1 < len)
        {
            char n = enc[i + 1];
            switch (n)
            {
            case 'n':
                raw[j++] = '\n';
                i++;
                break;
            case 't':
                raw[j++] = '\t';
                i++;
                break;
            case 'r':
                raw[j++] = '\r';
                i++;
                break;
            case '\\':
                raw[j++] = '\\';
                i++;
                break;
            default:
                raw[j++] = n;
                i++;
                break;
            }
        }
        else
        {
            raw[j++] = enc[i];
        }
    }
    raw[j] = '\0';
    return raw;
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