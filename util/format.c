#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

char **split_str(char *str, char delimiter, bool rm_double_spaces,
                 bool parse_braces) {
  unsigned long size = strlen(str);
  char **str_arr = (char **)malloc(sizeof(char *) * (size + 1));
  int idx = 0;
  bool found_brace = false;
  int start = 0;
  bool inside_word = false;

  for (int i = 0; i < size; i++) {
    if (!parse_braces && found_brace)
      continue;

    else if (str[i] == '(') {
      found_brace = true;
    } else if (str[i] == ')') {
      found_brace = false;
    } else if (str[i] == delimiter) {
      if (inside_word) {
        int token_len = i - start;
        while (start < i && str[start] == ' ') start++;
        int end = i;
        while (end > start && str[end - 1] == ' ') end--;
        token_len = end - start;

        str_arr[idx] = (char *)malloc(token_len + 1);
        memcpy(str_arr[idx], str + start, token_len);
        str_arr[idx][token_len] = '\0';
        idx++;
        inside_word = false;
      }
      start = i + 1;
    } else if (str[i] == ' ' && !found_brace) {
      if (inside_word) {
        inside_word = false;
      }
    } else {
      if (!inside_word) {
        inside_word = true;
        start = i;
      }
    }
  }

  if (start < size) {
    int token_len = size - start;
    while (start < size && str[start] == ' ') start++;
    int end = size;
    while (end > start && str[end - 1] == ' ') end--;

    if (end > start && str[end - 1] == ')') {
      end--;
    }

    token_len = end - start;

    str_arr[idx] = (char *)malloc(token_len + 1);
    memcpy(str_arr[idx], str + start, token_len);
    str_arr[idx][token_len] = '\0';
    idx++;
  }

  str_arr[idx] = NULL;

  return str_arr;
}

int leading_space_count(char *str) {
  int count = 0;
  for (int i = 0; i < strlen(str); i++) {
    if (str[i] != ' ') break;
    count++;
  }
  return count;
}

int trailing_space_count(char *str) {
  int count = 0;
  int length = strlen(str);

  for (int i = length - 1; i >= 0; i--) {
    if (str[i] == ' ') {
      count++;
    } else {
      break;
    }
  }

  return count;
}

bool includes(char *str, char ch) {
  for (int i = 0; i < strlen(str); i++) {
    if (str[i] == ch) {
      return true;
    }
  }
  return false;
}

bool ends_with(char *str, char ch) { return str[strlen(str) - 1] == ch; }
