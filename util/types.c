#include "types.h"

#include <ctype.h>
#include <stdbool.h>

#include "string.h"

bool is_str(char *str) {
  if (str == NULL || *str == '\0') {
    return false;
  }
  return (str[0] == '"' && str[strlen(str) - 1] == '"');
}

bool is_int(char *str) {
  if (str == NULL || *str == '\0') {
    return false;
  }
  while (*str) {
    if (!isdigit(*str)) {
      return false;
    }
    str++;
  }
  return true;
}

bool is_float(char *str) {
  if (str == NULL || *str == '\0') {
    return false;
  }

  bool found_point = false;
  bool has_digits = false;

  while (*str) {
    if (isdigit(*str)) {
      has_digits = true;
    } else if (*str == '.') {
      if (found_point) {
        return false;
      }
      found_point = true;
    } else {
      return false;
    }
    str++;
  }

  return has_digits;
}

DataType type_of(char *str) {
  if (is_str(str))
    return STR;
  else if (is_int(str))
    return INT;
  else if (is_float(str)) {
    return FLOAT;
  }
}

DataType to_type(const char *str) {
  if (strcmp(str, "int") == 0) {
    return INT;
  } else if (strcmp(str, "str") == 0) {
    return STR;
  } else if (strcmp(str, "float") == 0) {
    return FLOAT;
  }
}

char *type_to_str(DataType type) {
  if (type == INT) {
    return "int";
  } else if (type == STR) {
    return "str";
  } else if (type == FLOAT) {
    return "float";
  }

  return NULL;
}