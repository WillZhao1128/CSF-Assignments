/*
 * Testing the helper functions written in c_textsearch_fns.c and asm_textsearch_fns.S
 * CSF Assignment 2
 * Aidan Aug, Will Zhao
 * aaug1@jhu.edu, wzhao33@jhu.edu
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h> // these functions may be used in the test program
#include "tctest.h"
#include "textsearch_fns.h"

typedef struct {
  const char *pandp;
} TestObjs;


TestObjs *setup(void);
void cleanup(TestObjs *objs);

// TODO: declare test functions

// Example:
void test_read_line(TestObjs *objs);
void test_print_line();
void test_count_occurrences(TestObjs *objs);
void test_find_string_length();
void test_strings_equal(TestObjs *objs);
void test_handle_arguments();
void test_calc_total_occurrences(TestObjs *objs);


int main(int argc, char **argv) {
  // Allow the name of the test function to execute to be specified
  // on the command line
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  // TODO: invoke test functions
  
  TEST(test_read_line);
  
  //TEST(test_print_line);
  //TEST(test_count_occurrences);
  //TEST(test_find_string_length);
  //TEST(test_strings_equal);
  //TEST(test_handle_arguments);
  TEST(test_calc_total_occurrences);
  
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


// TODO: implementations of other test functions
void test_print_line() {
  char buf1[512];
  FILE *out = fmemopen(buf1, sizeof(buf1), "w");
  print_line(out, "qweqwassdd");
  fclose(out);

  out = fmemopen(buf1, sizeof(buf1), "r");
  ASSERT(0 == strcmp(buf1, "qweqwassdd"));
  fclose(out);


  char buf2[512];
  out = fmemopen(buf2, sizeof(buf2), "w");
  print_line(out, " ");
  fclose(out);

  out = fmemopen(buf2, sizeof(buf2), "r");
  ASSERT(0 == strcmp(buf2, " "));
  fclose(out);

  char buf3[512];
  out = fmemopen(buf3, sizeof(buf3), "w");
  print_line(out, " asd qwe  aaa \n as");
  fclose(out);

  out = fmemopen(buf3, sizeof(buf3), "r");
  ASSERT(0 == strcmp(buf3, " asd qwe  aaa \n as"));
  fclose(out);

  char buf4[512];
  out = fmemopen(buf4, sizeof(buf4), "w");
  print_line(out, "             \n\n\n aaaaas a");
  fclose(out);

  out = fmemopen(buf4, sizeof(buf4), "r");
  ASSERT(0 == strcmp(buf4, "             \n\n\n aaaaas a"));
  fclose(out);
  

}


void test_count_occurrences(TestObjs *objs) {
  FILE *in = fmemopen((char *) objs->pandp, strlen(objs->pandp), "r");

  char* hello = "hello";
  //ASSERT(count_occurrences(hello, "h") == 1);

  char* buf = malloc(sizeof(char) * 512);
  read_line(in, buf);
  ASSERT(count_occurrences(buf, "a") == 6);
  ASSERT(count_occurrences(buf, "It") == 1);
  ASSERT(count_occurrences(buf, " in") == 1);
  ASSERT(count_occurrences(buf, " inas") == 0);

  read_line(in, buf);
  ASSERT(count_occurrences(buf, "pos") == 1);
  ASSERT(count_occurrences(buf, "wife.") == 1);
  ASSERT(count_occurrences(buf, " ") == 11);
  ASSERT(count_occurrences(buf, "  ") == 0);

  read_line(in, buf);
  ASSERT(count_occurrences(buf, "pos") == 0);
  ASSERT(count_occurrences(buf, "wife.") == 0);
  ASSERT(count_occurrences(buf, "sqwasd") == 0);
  ASSERT(count_occurrences(buf, "  a") == 0);

  free(buf);
  fclose(in);

}


void test_find_string_length() {
  ASSERT(find_string_length("") == strlen(""));
  ASSERT(find_string_length("absa") == strlen("aasd"));
  ASSERT(find_string_length("22a a ") == strlen("22 b b"));
  ASSERT(find_string_length("     ") == strlen("12333"));
  ASSERT(find_string_length("I am testing this function called find_string_length") == strlen("I am testing this function called find_string_length"));
  ASSERT(find_string_length("I am testing this function called find_string_length\n Hopefully this works as intended") == strlen("I am testing this function called find_string_length\n Hopefully this works as intended"));
  ASSERT(find_string_length("\n\naa\n\n22\n") == strlen("\n\naa\n\n22\n"));
  ASSERT(find_string_length("\0") == strlen("\0"));
  ASSERT(find_string_length("00\0") == strlen("00\0"));
  ASSERT(find_string_length("00000") == strlen("00000"));
  ASSERT(find_string_length("I am testing this function called find_string_length\nI am testing this function called find_string_length\nI am testing this function called find_string_length\nI am testing this function called find_string_length\nI am testing this function called find_string_length\nI am testing this function called find_string_length\nI am testing this function called find_string_length\n") == strlen("I am testing this function called find_string_length\nI am testing this function called find_string_length\nI am testing this function called find_string_length\nI am testing this function called find_string_length\nI am testing this function called find_string_length\nI am testing this function called find_string_length\nI am testing this function called find_string_length\n"));
  ASSERT(find_string_length("     \n   ") == strlen("     \n   "));
}

void test_strings_equal(TestObjs *objs) {
  FILE *in = fmemopen((char *) objs->pandp, strlen(objs->pandp), "r");

  char* buf = malloc(sizeof(char) * 512);
  read_line(in, buf);
  ASSERT(strings_equal(buf, "It is a truth universally acknowledged, that a single man in") == 1);
  ASSERT(strings_equal(buf, "It is ") == 1);
  ASSERT(strings_equal(buf, "I") == 1);
  ASSERT(strings_equal(buf, "t is ") == 0);
  ASSERT(strings_equal(buf, "a") == 0);

  read_line(in, buf);
  ASSERT(strings_equal(buf, "possession of a good fortune, must be in want of a wife.") == 1);
  ASSERT(strings_equal(buf, "po") == 1);
  ASSERT(strings_equal(buf, "possession of a ") == 1);
  ASSERT(strings_equal(buf, "ossession of a good fortune, must be in want of a wife.") == 0);
  ASSERT(strings_equal(buf, ".") == 0);
  ASSERT(strings_equal(buf, "") == 1);

  read_line(in, buf);
  ASSERT(strings_equal(buf, "") == 1);
  ASSERT(strings_equal(buf, "  ") == 0);
  ASSERT(strings_equal(buf, "as") == 0);
  ASSERT(strings_equal(buf, ".") == 0);
  ASSERT(strings_equal(buf, " ") == 0);

  free(buf);
  fclose(in);
}



void test_handle_arguments(){
  ASSERT(handle_arguments(3) == 2);
  ASSERT(handle_arguments(4) == 3);
}



void test_calc_total_occurrences(TestObjs *objs){
  FILE *in = fmemopen((char *) objs->pandp, strlen(objs->pandp), "r");
  ASSERT(calc_total_occurrences(in, "a", 4) == 17);
  rewind(in);
  ASSERT(calc_total_occurrences(in, "considered", 4) == 1);
  rewind(in);
  ASSERT(calc_total_occurrences(in, "universally acknowledged, t", 4) == 1);
  rewind(in);
  ASSERT(calc_total_occurrences(in, "ed", 4) == 4);
  rewind(in);
  ASSERT(calc_total_occurrences(in, "considered a", 4) == 1);
  
  fclose(in);
}
