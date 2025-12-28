#ifndef TEXT_H
#define TEXT_H

#include <stddef.h>

char *text_read_line(const char *prompt); // malloc
int text_read_int_range(const char *prompt, int min, int max, int *out);

char *text_encode_one_line(const char *raw); // malloc
char *text_decode_one_line(const char *enc); // malloc

/**
 * Checks if the given needle is a substring of haystack, case-insensitive.
 */
int text_contains_case_insensitive(const char *haystack, const char *needle);

#endif