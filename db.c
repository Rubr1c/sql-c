#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct {
    char *table_name;
    char **cols;
    int col_count;
    char ***rows;
    int row_count;
    int row_capacity;
} Table;


struct TableNode {
    Table *table;
    struct TableNode *next;
};

struct TableNode *table_list_head = NULL;
struct TableNode *table_list_tail = NULL;
int table_count = 0;

struct TableNode* new_table_node(Table *table, struct TableNode *next) {
    struct TableNode *node = (struct TableNode *)malloc(sizeof(struct TableNode));
    node->table = table;
    node->next = next;
    return node;
}



void add_table(Table* table) {
    if (table_count == 0) {
        table_list_head = table_list_tail = new_table_node(table, NULL);
    } else {
        table_list_tail->next = new_table_node(table, NULL);
        table_list_tail= table_list_tail->next;
    }
    table_count++;
}

Table* get_table(char *name) {
    struct TableNode *curr = table_list_head;
    while (curr != NULL) {
        if (strcmp(curr->table->table_name, name) == 0) {
            return curr->table;
        }
        curr = curr->next;
    }
    return NULL;
}

void print_tables() {
    struct TableNode* curr = table_list_head;
    while (curr != NULL) {
        printf("%s->", curr->table->table_name);
        curr = curr->next;
    }
}

void free_table_list() {
    struct TableNode *current = table_list_head;
    while (current != NULL) {
        struct TableNode *next = current->next;

        free(current->table->table_name);

        for (int i = 0; i < current->table->col_count; i++) {
            free(current->table->cols[i]);
        }
        free(current->table->cols);
        for (int i = 0; i < current->table->row_count; i++) {
            for (int j = 0; j < current->table->col_count; j++) {
                free(current->table->rows[i][j]);
            }
        }

        free(current->table);

        free(current);

        current = next;
    }

    table_list_head = NULL;
    table_list_tail = NULL;
    table_count = 0;
}


char** split_str(char *str, char delimiter, bool rm_double_spaces, bool parse_braces) {
    unsigned long size = strlen(str);
    char **str_arr = (char **)malloc(sizeof(char*) * (size + 1)); 
    int idx = 0;
    bool found_brace = false;
    int start = 0;
    bool inside_word = false;

    for (int i = 0; i < size; i++) {
        if (!parse_braces && found_brace) continue;

        else if (str[i] == '(') {
            found_brace = true;
        }
        else if (str[i] == ')'){
            found_brace = false; 
        } 
        else if (str[i] == delimiter) {
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
        }
        else if (str[i] == ' ' && !found_brace) {
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

void add_row(Table *table, char **row) {
    if (table->row_count >= table->row_capacity) {
        table->rows = realloc(table->rows, sizeof(char*) * table->row_capacity * 2);
        table->row_capacity *= 2;
    }
    table->rows[table->row_count++] = row;
}

void print_rows(Table *t) {
    int *col_max_lengths = malloc(sizeof(int) * t->col_count);
    
    // Calculate the maximum length for each column
    for (int j = 0; j < t->col_count; j++) {
        col_max_lengths[j] = strlen(t->cols[j]);  // Start with column name length
        for (int i = 0; i < t->row_count; i++) {
            int cell_length = strlen(t->rows[i][j]);
            if (cell_length > col_max_lengths[j]) {
                col_max_lengths[j] = cell_length;
            }
        }
    }

    // Print column headers with proper alignment
    printf("       ");
    for (int i = 0; i < t->col_count; i++) {
        printf("%-*s   ", col_max_lengths[i], t->cols[i]);
    }
    printf("\n");

    // Print the rows with proper alignment
    for (int i = 0; i < t->row_count; i++) {
        printf("row %i: ", i);
        for (int j = 0; j < t->col_count; j++) {
            printf("%-*s   ", col_max_lengths[j], t->rows[i][j]);
        }
        printf("\n");
    }

    free(col_max_lengths);  // Don't forget to free the dynamically allocated memory
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


void parse_command(char *str) {
    char **tokens = split_str(str, ' ', true, false);

    if (strcmp(tokens[0], "CREATE") == 0) {
        if (strcmp(tokens[1], "TABLE") == 0) {
            Table *table = (Table *)malloc(sizeof(Table));

            table->table_name = malloc(strlen(tokens[2]) + 1);
            strcpy(table->table_name, tokens[2]);

            char** cols = split_str(tokens[3], ',', true, true);
            int col_count = 0;
            while (cols[col_count] != NULL) {
                col_count++;
            }

            table->cols = cols;
            table->col_count = col_count;

            table->rows = malloc(sizeof(char*) * 10);
            table->row_capacity = 10;

            table->row_count = 0;

            add_table(table);
            printf("Table %s created!\n", tokens[2]);
        }
    } else if (strcmp(tokens[0], "INSERT") == 0) {
        if (strcmp(tokens[1], "INTO") == 0) {
           Table* t = get_table(tokens[2]); 
           if (strcmp(tokens[3], "VALUES") == 0) {
                char **values = split_str(tokens[4], ',', true, true);
                add_row(t, values); 
           }
        }
    } else if (strcmp(tokens[0], "SELECT") == 0) {
        if (strcmp(tokens[1], "*") == 0) {
            if (strcmp(tokens[2], "FROM") == 0) {
                Table * t = get_table(tokens[3]);
                print_rows(t);
            }
        }
    }
    
    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

