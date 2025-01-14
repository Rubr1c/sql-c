#include <stdbool.h>

#include "../table/table.h"

#ifndef DB_H
#define DB_H

typedef struct {
  char name[256];
  struct TableNode *head;
  struct TableNode *tail;
  int table_count;
} DB;

struct DBNode {
  DB *db;
  struct DBNode *next;
};

DB *new_db(char *name);
DB *register_db(DB *db);
DB *get_db(char *name);
bool remove_db(char *name);
void print_db();
void add_table(DB *db, Table *table);
Table *get_table(DB *db, char *name);
bool remove_table(DB *db, const char *table_name);
void print_tables(DB *db);
void free_table_list(DB *db);


#endif
