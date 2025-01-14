#include "db.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../table/table.h"
#include "db.h"

struct DBNode *db_head = NULL;
struct DBNode *db_tail = NULL;
int db_size = 0;

struct DBNode *new_db_node(DB *db, struct DBNode *next) {
  struct DBNode *node = (struct DBNode *)malloc(sizeof(struct DBNode));
  node->db = db;
  node->next = next;
  return node;
}

DB *register_db(DB *db) {
  if (db_size == 0) {
    db_head = db_tail = new_db_node(db, NULL);
  } else {
    db_tail->next = new_db_node(db, NULL);
    db_tail = db_tail->next;
  }
  db_size++;
  return db;
}

DB *get_db(char *name) {
  struct DBNode *curr = db_head;
  while (curr != NULL) {
    if (strcmp(curr->db->name, name) == 0) {
      return curr->db;
    }
    curr = curr->next;
  }
  return NULL;
}

DB *new_db(char *name) {
  DB *db = (DB *)malloc(sizeof(DB));
  strcpy(db->name, name);
  db->table_count = 0;
  db->head = NULL;
  db->tail = NULL;
  return db;
}

void free_db(DB *db) {
  free(db->name);
  db->head = NULL;
  db->tail = NULL;
  free(db->head);
  free(db->tail);
}

void free_table(Table *table) {
  free(table->table_name);

  for (int i = 0; i < table->col_count; i++) {
    free(table->cols[i]);
    free((void *)table->types[i]);
  }
  free(table->cols);
  for (int i = 0; i < table->row_count; i++) {
    for (int j = 0; j < table->col_count; j++) {
      free(table->rows[i][j]);
    }
  }

  free(table);
}

bool remove_tables(DB *db) {
  struct TableNode *curr = db->head;
  while (curr != NULL) {
    struct TableNode *temp = curr;
    curr = curr->next;
    free_table(temp->table);
    free(temp);
  }
}

bool remove_db(char *name) {
  struct DBNode *curr = db_head;
  if (strcmp(curr->db->name, name) == 0) {
    struct DBNode *temp = db_head;
    db_head = db_head->next;
    for (int i = 0; i < temp->db->table_count; i++) {
      remove_tables(temp->db);
    }
    free_db(temp->db);
    free(temp);
    return true;
    db_size--;
  }

  while (curr->next != NULL) {
    if (strcmp(curr->db->name, name) == 0) {
      struct DBNode *temp = curr->next;
      curr->next = curr->next->next;
      free_db(temp->db);
      free(temp);
      return true;
      db_size--;
    }
  }
  return false;
}

void print_db() {
  struct DBNode *curr = db_head;
  while (curr != NULL) {
    printf("DB: %s - %i tables\n", curr->db->name, curr->db->table_count);
    curr = curr->next;
  }
}

struct TableNode *new_table_node(Table *table, struct TableNode *next) {
  struct TableNode *node = (struct TableNode *)malloc(sizeof(struct TableNode));
  node->table = table;
  node->next = next;
  return node;
}

void add_table(DB *db, Table *table) {
  if (db->table_count == 0) {
    db->head = db->tail = new_table_node(table, NULL);
  } else {
    db->tail->next = new_table_node(table, NULL);
    db->tail = db->tail->next;
  }
  db->table_count++;
}

bool remove_table(DB *db, const char *table_name) {
  struct TableNode *curr = db->head;
  if (strcmp(curr->table->table_name, table_name) == 0) {
    struct TableNode *temp = db->head;
    db->head = db->head->next;
    free_table(temp->table);
    temp->next = NULL;
    free(temp);
    return true;
    db->table_count--;
  }

  while (curr->next != NULL) {
    if (strcmp(curr->next->table->table_name, table_name) == 0) {
      struct TableNode *temp = curr->next;
      curr->next = curr->next->next;
      free_table(temp->table);
      temp->next = NULL;
      free(temp);
      return true;
      db->table_count--;
    }
  }
  return false;
}

Table *get_table(DB *db, char *name) {
  struct TableNode *curr = db->head;
  while (curr != NULL) {
    if (strcmp(curr->table->table_name, name) == 0) {
      return curr->table;
    }
    curr = curr->next;
  }
  return NULL;
}

void print_tables(DB *db) {
  struct TableNode *curr = db->head;
  while (curr != NULL) {
    printf("%s->", curr->table->table_name);
    curr = curr->next;
  }
}

void free_table_list(DB *db) {
  struct TableNode *current = db->head;
  while (current != NULL) {
    struct TableNode *next = current->next;

    free_table(current->table);

    free(current);

    current = next;
  }

  db->head = NULL;
  db->tail = NULL;
}