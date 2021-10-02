/*
 * C language implementation of the main function for mimicing unix grep function
 * CSF Assignment 2
 * Aidan Aug, Will Zhao
 * aaug1@jhu.edu, wzhao33@jhu.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include "textsearch_fns.h"


int main(int argc, char **argv) {
  // TODO: implement
  int file_ind = handle_arguments(argc);

  FILE* fileptr = fopen(argv[file_ind], "r");
  if (fileptr == NULL) {
    fprintf(stderr, "Could not open file\n");
    exit(2);
  }
  
  int tot_occurrences = calc_total_occurrences(fileptr, argv[file_ind - 1], argc);

  if (argc == 4) {
    char* c_flag = "-c";
    if (strings_equal(argv[1], c_flag)) {
      fclose(fileptr);
      fprintf(stdout, "%d occurrence(s)\n", tot_occurrences);
      return 0;
    } else {
      fclose(fileptr);
      fprintf(stderr, "Invalid input\n");
      exit(1);
    }
  }

  fclose(fileptr);
  return 0;
}