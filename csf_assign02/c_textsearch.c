#include <stdio.h>
#include <stdlib.h>
#include "textsearch_fns.h"


int main(int argc, char **argv) {
  // TODO: implement
  int file_ind = handle_arguments(argc);

  FILE* fileptr = fopen(argv[file_ind], "r");
  if (fileptr == NULL) {
    fprintf(stderr, "Could not open file");
    exit(2);
  }
  
  int tot_occurrences = calc_total_occurences(fileptr, argv[file_ind - 1], argc);

  if (argc == 4) {
    char* c_flag = "-c";
    if (strings_equal(argv[1], c_flag)) {
      fprintf(stdout, "%d occurrence(s)", tot_occurrences);
      exit(0);
    } else {
      printf("argv[1]: %s\n", argv[1]);
      fprintf(stderr, "Invalid input");
      exit(1);
    }
  }

  fclose(fileptr);
  return 0;
}