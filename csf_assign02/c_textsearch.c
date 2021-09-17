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
  
  int flag = 1;
  int tot_occurrences;
  char buff[512] = {0};

  while (flag) {
    char* buff = malloc(sizeof(char) * 512);
    flag = read_line(fileptr, buff);
    tot_occurrences += count_occurrences(buff, argv[1]);
    //printf("%s", buff);
  }

  printf("%d", tot_occurrences);
  

  fclose(fileptr);
  return 0;
}
