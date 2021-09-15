#include <stdio.h>
#include <stdlib.h>
#include <string.h> // these functions may be used in the test program
#include "textsearch_fns.h"
#include "tctest.h"

typedef struct {
  const char *pandp;
} TestObjs;


TestObjs *setup(void);
void cleanup(TestObjs *objs);

// TODO: declare test functions

// Example:
// void test_read_line(TestObjs *objs);


int main(int argc, char **argv) {
  // Allow the name of the test function to execute to be specified
  // on the command line
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  // TODO: invoke test functions
  // TEST(test_read_line);

  TEST_FINI();

  return 0;
}


TestObjs *setup(void) {
  TestObjs *objs = malloc(sizeof(TestObjs));

  objs->pandp =
    "It is a truth universally acknowledged, that a single man in\n"
    "possession of a good fortune, must be in want of a wife.\n"
    "\n"
    "However little known the feelings or views of such a man may be\n"
    "on his first entering a neighbourhood, this truth is so well\n"
    "fixed in the minds of the surrounding families, that he is\n"
    "considered as the rightful property of some one or other of their\n"
    "daughters.\n";

  return objs;
}

void cleanup(TestObjs *objs) {
  free(objs);
}

// An example test function
/*
void test_read_line(TestObjs *objs) {
  // the fmemopen function allows us to treat a character string
  // as an input file
  FILE *in = fmemopen((char *) objs->pandp, strlen(objs->pandp), "r");
  char buf[MAXLINE + 1];

  ASSERT(read_line(in, buf));
  ASSERT(0 == strcmp(buf, "It is a truth universally acknowledged, that a single man in"));

  ASSERT(read_line(in, buf));
  ASSERT(0 == strcmp(buf, "possession of a good fortune, must be in want of a wife."));

  ASSERT(read_line(in, buf));
  ASSERT(0 == strcmp(buf, ""));

  ASSERT(read_line(in, buf));
  ASSERT(0 == strcmp(buf, "However little known the feelings or views of such a man may be"));

  ASSERT(read_line(in, buf));
  ASSERT(0 == strcmp(buf, "on his first entering a neighbourhood, this truth is so well"));

  ASSERT(read_line(in, buf));
  ASSERT(0 == strcmp(buf, "fixed in the minds of the surrounding families, that he is"));

  ASSERT(read_line(in, buf));
  ASSERT(0 == strcmp(buf, "considered as the rightful property of some one or other of their"));

  ASSERT(read_line(in, buf));
  ASSERT(0 == strcmp(buf, "daughters."));

  // at this point we have read the last line
  ASSERT(!read_line(in, buf));

  fclose(in);
}
*/

// TODO: implementations of other test functions
