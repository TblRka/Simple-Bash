#include <stdio.h>

#include "s21_grep_lib.h"

int main(int argc, char **argv) {
  struct grep *action = calloc(1, sizeof(struct grep));
  command_processing(argc, argv, action);
  int return_value = 0;
  if (action->error == 0) {
    // print_action(action);
    char **ptr = action->file_name;
    while (*ptr != NULL) {
      FILE *fp = fopen(*ptr, "r");
      if (fp != NULL) {
        grep_file(fp, action, *ptr);
        fclose(fp);
      } else {
        if (!(action->s)) {
          fprintf(stderr, "grep: %s: no such file or directory\n", *ptr);
        }
        return_value = 1;
      }
      ptr++;
    }
  } else {
    return_value = 1;
  }
  free(action->e_patterns);
  free(action);
  return return_value;
}
