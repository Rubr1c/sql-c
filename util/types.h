#include <stdbool.h>

#ifndef TYPES_H
#define TYPES_H

typedef enum { INT, FLOAT, STR } DataType;

DataType type_of(char *str);
bool is_str(char *str);
bool is_int(char *str);
bool is_float(char *str);
DataType to_type(const char *str);
char *type_to_str(DataType type);

#endif
