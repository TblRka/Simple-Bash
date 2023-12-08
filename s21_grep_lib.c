#include "s21_grep_lib.h"

void command_processing(int argc, char **argv, struct grep *action) {
  if (argc < 4) {
    action->error = 1;
  }
  action->e_patterns = calloc(argc - 2, sizeof(char *));
  int rez = 0;
  while ((rez = getopt(argc, argv, "e:ivclnhs")) != -1) {
    switch (rez) {
      case 'e': {
        action->e_patterns[action->e] = optarg;
        action->e += 1;
        break;
      }
      case 'i': {
        action->i += 1;
        break;
      }
      case 'v': {
        action->v += 1;
        break;
      }
      case 'c': {
        if (!(action->l)) {
          action->c += 1;
          action->n = 0;
        }
        break;
      }
      case 'l': {
        action->l += 1;
        break;
      }
      case 'n': {
        if (!(action->c)) {
          action->n += 1;
        }
        break;
      }
      case 's': {
        action->s += 1;
        break;
      }
      case 'h': {
        action->h += 1;
        break;
      }
      case '?':
      default: {
        action->error = 1;
        break;
      }
    }
  }
  if (!(action->e)) {
    action->pattern = argv + optind;
    action->file_name = argv + optind + 1;
    action->file_count = argc - optind - 1;
  } else {
    action->file_name = argv + optind;
    action->file_count = argc - optind;
  }
  // printf("%d %d", argc, optind);
}

void grep_file(FILE *input, struct grep *action, char *file_ptr) {
  ssize_t read;
  size_t len = 0;
  char *istr = NULL;
  int i = 1;
  int count = 0;
  int sol;
  while ((read = getline(&istr, &len, input)) != -1) {
    sol = recognition_mode(action, istr);
    if (!(action->l) && !(action->c) && sol) {
      if (action->file_count > 1 && !(action->h)) {
        printf("%s:", file_ptr);
      }
      if (action->n) {
        printf("%d:", i);
      }
      if (istr[strlen(istr) - 1] == '\n') {
        printf("%s", istr);
      } else {
        printf("%s\n", istr);
      }
    }
    if (sol) {
      count += 1;
    }
    i++;
  }
  free(istr);

  if (action->l && count) {
    printf("%s\n", file_ptr);
  }

  if (action->c && action->file_count > 1) {
    printf("%s:%d\n", file_ptr, count);
  } else if (action->c && action->file_count == 1) {
    printf("%d\n", count);
  }
}

int recognition_mode(struct grep *action, char *istr) {
  regex_t regex;
  int solution = 0;
  int return_value;
  if (action->e) {
    for (int i = 0; i < action->e; i++) {
      if (action->i) {
        regcomp(&regex, (action->e_patterns)[i], REG_ICASE);
      } else {
        regcomp(&regex, (action->e_patterns)[i], 0);
      }
      return_value = regexec(&regex, istr, 0, NULL, 0);
      if (action->v != 0 && return_value == REG_NOMATCH) {
        solution += 1;
      } else if (action->v == 0 && return_value == 0) {
        solution += 1;
      }
      regfree(&regex);
    }
    if (action->v != 0 && solution != action->e) {
      solution = 0;
    }
  } else {
    if (action->i) {
      regcomp(&regex, *(action->pattern), REG_ICASE);
    } else {
      regcomp(&regex, *(action->pattern), 0);
    }

    return_value = regexec(&regex, istr, 0, NULL, 0);
    if (action->v != 0 && return_value == REG_NOMATCH) {
      solution = 1;
    } else if (action->v == 0 && return_value == 0) {
      solution = 1;
    }
    regfree(&regex);
  }
  return solution;
}

void print_action(struct grep *action) {
  printf("flag e: %d\n", action->e);
  printf("flag i: %d\n", action->i);
  printf("flag v: %d\n", action->v);
  printf("flag c: %d\n", action->c);
  printf("flag l: %d\n", action->l);
  printf("flag n: %d\n", action->n);
  printf("files count: %d\n", action->file_count);

  if (action->e) {
    char **pattern_ptr = action->e_patterns;
    while (*pattern_ptr != NULL) {
      printf("e pattern: %s\n", *pattern_ptr);
      pattern_ptr++;
    }
  } else {
    printf("pattern: %s\n", *(action->pattern));
  }

  char **file_ptr = action->file_name;
  while (*file_ptr != NULL) {
    printf("file: %s\n", *file_ptr);
    file_ptr++;
  }
}