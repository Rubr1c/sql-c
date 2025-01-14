#include <stdbool.h>

#ifndef FORMAT_H
#define FORMAT_H

char **split_str(char *str, char delimiter, bool rm_double_spaces,
                 bool parse_braces);
int leading_space_count(char *str);
int trailing_space_count(char *str);
bool includes(char *str, char ch);
bool ends_with(char *str, char ch);

#endif