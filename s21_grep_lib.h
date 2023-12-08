#ifndef S21_GREP_LIB_H
#define S21_GREP_LIB_H
#include <getopt.h>
#include <regex.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct grep {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;

  int h;
  int s;
  int f;
  int o;

  int error;
  int no_flags;
  char **e_patterns;
  char **pattern;
  char **file_name;
  int file_count;
};

void command_processing(int command_len, char **command, struct grep *action);
void grep_file(FILE *input, struct grep *action, char *file_ptr);
int recognition_mode(struct grep *action, char *istr);
void print_action(struct grep *action);
#endif  // S21_GREP_LIB_H