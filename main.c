#include <stdio.h>
#include <string.h>

#include "db.h"

int main() {
  // char command[256];

  // while (1) {
  //   printf("> ");
  //   if (fgets(command, sizeof(command), stdin) == NULL) {
  //     printf("Error reading input.\n");
  //     continue;
  //   }

  //   command[strcspn(command, "\n")] = '\0';

  //   if (strcmp(command, "exit") == 0) {
  //     printf("Exiting...\n");
  //     break;
  //   } else {
  //     parse_command(command);
  //   }
  // }
  parse_command("CREATE DB bold");
  parse_command("CREATE TABLE cold (id: int, name: str, age: float)");
  parse_command("DESCRIBE cold");
  // parse_command("INSERT INTO cold VALUES (1, \"rubric\", 19)");
  // parse_command("INSERT INTO cold VALUES (2, hamsa, 5)");
  // parse_command("DELETE FROM cold WHERE id  =2");
  // parse_command("SELECT * FROM cold");
}
