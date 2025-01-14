#include <stdbool.h>

#include "../util/types.h"

#ifndef TABLE_H
#define TABLE_H

typedef struct {
  char *table_name;
  char **cols;
  DataType *types;
  int col_count;
  char ***rows;
  int row_count;
  int row_capacity;
} Table;

struct TableNode {
  Table *table;
  struct TableNode *next;
};

bool add_row(Table *table, char **row);
void print_rows(Table *t);
int cremove_row(Table *table, char *col, char *value);
void describe(Table *table);

#endif
