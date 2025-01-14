#include "table.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../util/types.h"

bool valid_row(Table *table, char **row) {
  for (int i = 0; i < table->col_count; i++) {
    printf("Checking %s == %s\n", type_to_str(table->types[i]), row[i]);
    if (table->types[i] != type_of(row[i])) {
      return false;
    }
  }
  return true;
}

bool add_row(Table *table, char **row) {
  if (table->row_count >= table->row_capacity) {
    table->rows =
        realloc(table->rows, sizeof(char *) * table->row_capacity * 2);
    table->row_capacity *= 2;
  }
  if (valid_row(table, row)) {
    table->rows[table->row_count++] = row;
    return true;
  } else {
    return false;
  }
}

void tremove_row(Table *table, int idx) {
  if (idx < 0 || idx >= table->row_count) {
    printf("Invalid row index.\n");
    return;
  }

  for (int i = idx; i < table->row_count - 1; i++) {
    table->rows[i] = table->rows[i + 1];
  }

  table->rows[table->row_count - 1] = NULL;
  table->row_count--;
}

int cremove_row(Table *table, char *col, char *value) {
  int count = 0;
  for (int i = 0; i < table->col_count; i++) {
    if (strcmp(table->cols[i], col) == 0) {
      for (int j = 0; j < table->row_count; j++) {
        if (strcmp(table->rows[j][i], value) == 0) {
          tremove_row(table, j);
          j--;
          count++;
        }
      }
      break;
    }
  }
  return count;
}

void print_rows(Table *t) {
  int *col_max_lengths = malloc(sizeof(int) * t->col_count);
  for (int j = 0; j < t->col_count; j++) {
    col_max_lengths[j] = strlen(t->cols[j]);
    for (int i = 0; i < t->row_count; i++) {
      int cell_length = strlen(t->rows[i][j]);

      if (cell_length > col_max_lengths[j]) {
        col_max_lengths[j] = cell_length;
      }
    }
  }

  printf("       ");
  for (int i = 0; i < t->col_count; i++) {
    printf("%-*s   ", col_max_lengths[i], t->cols[i]);
  }
  printf("\n");

  for (int i = 0; i < t->row_count; i++) {
    printf("row %i: ", i);
    for (int j = 0; j < t->col_count; j++) {
      printf("%-*s   ", col_max_lengths[j], t->rows[i][j]);
    }
    printf("\n");
  }
  free(col_max_lengths);
}

void describe(Table *table) {
  printf("Table: %s\n", table->table_name);
  printf("Columns:\n");
  for (int i = 0; i < table->col_count; i++) {
    printf("    - %s : %s\n", table->cols[i], type_to_str(table->types[i]));
  }
}