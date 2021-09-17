#include <stdio.h>
#include <stdlib.h>
#include "textsearch_fns.h"


int main(int argc, char **argv) {
  // TODO: implement
  FILE* fileptr = fopen(argv[2], "r");
  if (fileptr == NULL) {
    fprintf(stderr, "Could not open file");
    return 1;
  }
  
  int flag = 0;
  char* buff;
  int tot_occurrences;

  while (flag != -1) {
    buff = malloc(sizeof(char) * 512);
    flag = read_line(fileptr, buff);
    tot_occurrences += count_occurrences(buff, argv[1]);
  }

  printf("%d", tot_occurrences);
  free(buff);

  fclose(fileptr);
  return 0;
}
