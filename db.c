#include "./db/db.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./table/table.h"
#include "./util/format.h"

DB *current_db = NULL;

void parse_command(char *str) {
  char **tokens = split_str(str, ' ', true, false);
  if (strcmp(tokens[0], "CREATE") == 0) {
    if (strcmp(tokens[1], "TABLE") == 0) {
      if (current_db == NULL) {
        printf("No database selected");
        return;
      }
      Table *table = (Table *)malloc(sizeof(Table));
      table->table_name = malloc(strlen(tokens[2]) + 1);
      strcpy(table->table_name, tokens[2]);
      char **cols = split_str(tokens[3], ',', true, true);
      int col_count = 0;
      while (cols[col_count] != NULL) {
        col_count++;
      }

      table->cols = malloc(col_count);
      table->types = malloc(sizeof(DataType) * col_count);

      for (int i = 0; i < col_count; i++) {
        char **types = split_str(cols[i], ':', true, false);
        table->cols[i] = types[0];
        table->types[i] = to_type(types[1]);
      }

      table->col_count = col_count;

      table->rows = malloc(sizeof(char *) * 10);
      table->row_capacity = 10;
      table->row_count = 0;

      add_table(current_db, table);

      printf("Table %s created!\n", tokens[2]);
    } else if (strcmp(tokens[1], "DB") == 0) {
      current_db = register_db(new_db(tokens[2]));

      printf("Created Database: %s\n", tokens[2]);
    }
  } else if (strcmp(tokens[0], "INSERT") == 0) {
    if (strcmp(tokens[1], "INTO") == 0) {
      if (current_db == NULL) {
        printf("No database selected\n");
        return;
      }
      Table *t = get_table(current_db, tokens[2]);
      if (t == NULL) {
        printf("Table '%s' does not exist in Database '%s'\n", tokens[2],
               current_db->name);
      }
      if (strcmp(tokens[3], "VALUES") == 0) {
        char **values = split_str(tokens[4], ',', true, true);
        if (!add_row(t, values)) {
          printf("Invaild Type\n");
        }
      }
    }
  } else if (strcmp(tokens[0], "SELECT") == 0) {
    if (strcmp(tokens[1], "*") == 0) {
      if (strcmp(tokens[2], "FROM") == 0) {
        if (current_db == NULL) {
          printf("No database selected");
          return;
        }
        Table *t = get_table(current_db, tokens[3]);
        if (t == NULL) {
          printf("Table '%s' does not exist in Database '%s'\n", tokens[3],
                 current_db->name);
        }
        print_rows(t);
      }
    }
  } else if (strcmp(tokens[0], "USE") == 0) {
    DB *db = get_db(tokens[1]);
    if (db == NULL) {
      printf("Database '%s' does not exist\n", tokens[1]);
      return;
    }
    current_db = db;
    printf("Using '%s'\n", tokens[1]);
  } else if (strcmp(tokens[0], "DROP") == 0) {
    if (strcmp(tokens[1], "TABLE") == 0) {
      if (!remove_table(current_db, tokens[2])) {
        printf("Table '%s' does not exist for Database '%s'\n", tokens[2],
               current_db->name);
        return;
      }
      printf("Dropped Table '%s'\n", tokens[2]);
    } else if (strcmp(tokens[1], "DB") == 0) {
      if (!remove_db(tokens[2])) {
        printf("Database '%s' does not exist\n", tokens[2]);
        return;
      }
      current_db = NULL;
      printf("Dropped Databse '%s'\n", tokens[2]);
    }
  } else if (strcmp(tokens[0], "DELETE") == 0) {
    if (strcmp(tokens[1], "FROM") == 0) {
      Table *t = get_table(current_db, tokens[2]);
      if (t == NULL) {
        printf("Table '%s' does not exist for Database '%s'\n", tokens[2],
               current_db->name);
        return;
      }
      if (strcmp(tokens[3], "WHERE") == 0) {
        int count;
        bool f_stuck = includes(tokens[4], '=');
        bool l_stuck = tokens[6] == NULL ? true : includes(tokens[6], '=');
        if (f_stuck && l_stuck) {
          char **split = split_str(tokens[4], '=', true, true);
          count = cremove_row(t, split[0], split[1]);
        } else if (f_stuck) {
          size_t size = strlen(tokens[4]);
          if (size > 0) {
            tokens[4][size - 1] = '\0';
            count = cremove_row(t, tokens[4], tokens[5]);
          }
        } else if (l_stuck) {
          strncpy(tokens[5], tokens[5] + 1, strlen(tokens[5]));
          count = cremove_row(t, tokens[4], tokens[5]);
        } else {
          count = cremove_row(t, tokens[4], tokens[6]);
        }

        printf("Removed %i row\\s\n", count);
      }
    }
  } else if (strcmp(tokens[0], "DESCRIBE") == 0) {
    Table *t = get_table(current_db, tokens[1]);
    if (t == NULL) {
      printf("Table '%s' does not exist for Database '%s'\n", tokens[1],
             current_db->name);
    }
    describe(t);
  }

  else {
    printf("Unknown command\n");
  }

  for (int i = 0; tokens[i] != NULL; i++) {
    free(tokens[i]);
  }
  free(tokens);
}
