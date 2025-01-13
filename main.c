#include "db.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main() {
    parse_command("CREATE TABLE cold (id, name, age)");
    parse_command("INSERT INTO cold VALUES (1, rubric, 19)");
    parse_command("INSERT INTO cold VALUES (2, hamsa, 5)");
    parse_command("SELECT * FROM cold");
 }
