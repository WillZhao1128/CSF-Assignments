/*
 * Fixedpoint Numeric Data Type and Arithmetic: Testing
 * CSF Assignment 1
 * Aidan Aug, Will Zhao
 * aaug1@jhu.edu, wzhao33@jhu.edu
 */


#include <stdio.h>
#include <stdlib.h>
#include "fixedpoint.h"
#include "tctest.h"

// Test fixture object, has some useful values for testing
typedef struct {
  Fixedpoint zero;
  Fixedpoint one;
  Fixedpoint one_half;
  Fixedpoint one_fourth;
  Fixedpoint large1;
  Fixedpoint large2;
  Fixedpoint large3;
  Fixedpoint min_magnitude;
  Fixedpoint max;
  Fixedpoint min;
  Fixedpoint half_max;
  Fixedpoint half_max_almost;

  // TODO: add more objects to the test fixture
  Fixedpoint rand1;
  Fixedpoint rand2;
  Fixedpoint rand3;
  Fixedpoint rand4;

} TestObjs;

// functions to create and destroy the test fixture
TestObjs *setup(void);
void cleanup(TestObjs *objs);

// test functions
void test_whole_part(TestObjs *objs);
void test_frac_part(TestObjs *objs);
void test_create_from_hex(TestObjs *objs);
void test_format_as_hex(TestObjs *objs);
void test_negate(TestObjs *objs);
void test_add(TestObjs *objs);
void test_sub(TestObjs *objs);
void test_is_overflow_pos(TestObjs *objs);
void test_is_err(TestObjs *objs);
// TODO: add more test functions
void test_add2(TestObjs *objs);
void test_create_from_hex2(TestObjs *objs);
void test_is_err2(TestObjs *objs);
void test_add3(TestObjs *objs);
void test_halve(TestObjs *objs);
void test_double(TestObjs *objs);
void test_is_overflow_pos2(TestObjs *objs);
void test_compare(TestObjs *objs);
void test_is_valid(TestObjs *objs);
void test_rby(TestObjs *objs);


int main(int argc, char **argv) {
  // if a testname was specified on the command line, only that
  // test function will be executed
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(test_whole_part);
  TEST(test_frac_part);
  TEST(test_create_from_hex);
  TEST(test_format_as_hex);
  TEST(test_negate);
  TEST(test_add);
  TEST(test_sub);
  TEST(test_is_overflow_pos);
  TEST(test_is_err);
  TEST(test_rby);

  // IMPORTANT: if you add additional test functions (which you should!),
  // make sure they are included here.  E.g., if you add a test function
  // "my_awesome_tests", you should add
  //
  //   TEST(my_awesome_tests);
  //
  // here. This ensures that your test function will actually be executed.
  TEST(test_create_from_hex2);
  TEST(test_is_err2);
  TEST(test_add2);
  TEST(test_add3);
  TEST(test_halve);
  TEST(test_double);
  TEST(test_is_overflow_pos2);
  TEST(test_compare);
  TEST(test_is_valid);
  TEST(test_rby);
  TEST_FINI();
}

TestObjs *setup(void) {
  TestObjs *objs = malloc(sizeof(TestObjs));

  objs->zero = fixedpoint_create(0UL);
  objs->one = fixedpoint_create(1UL);
  objs->one_half = fixedpoint_create2(0UL, 0x8000000000000000UL);
  objs->one_fourth = fixedpoint_create2(0UL, 0x4000000000000000UL);
  objs->large1 = fixedpoint_create2(0x4b19efceaUL, 0xec9a1e2418UL);
  objs->large2 = fixedpoint_create2(0xfcbf3d5UL, 0x4d1a23c24fafUL);
  objs->large3 = fixedpoint_create2(0x3fcbf3d5UL, 0xed1a23c79fa00000UL);
  objs->max = fixedpoint_create2(0xffffffffffffffffUL, 0xffffffffffffffffUL);
  objs->min = fixedpoint_create2(0UL, 0x1UL);
  objs->half_max = fixedpoint_create(0x8000000000000000UL);
  objs->half_max_almost = fixedpoint_create_from_hex("7FFFFFFFFFFFFFFF.7FFFFFFFFFFFFFFF");
  objs->rand1 = fixedpoint_create_from_hex("-1d00a3e9.2580b36847");
  objs->rand2 = fixedpoint_create_from_hex("1b3f576f1bc498.25c721eeace");
  objs->rand3 = fixedpoint_create_from_hex("-dff58c9aa.b9a");
  objs->rand4 = fixedpoint_create_from_hex("-1c20aaddcfd2.3ec8fadb45e52f");

  return objs;
}

void cleanup(TestObjs *objs) {
  free(objs);
}

void test_whole_part(TestObjs *objs) {
  ASSERT(0UL == fixedpoint_whole_part(objs->zero));
  ASSERT(1UL == fixedpoint_whole_part(objs->one));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_half));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_fourth));
  ASSERT(0x4b19efceaUL == fixedpoint_whole_part(objs->large1));
  ASSERT(0xfcbf3d5UL == fixedpoint_whole_part(objs->large2));
}

void test_frac_part(TestObjs *objs) {
  ASSERT(0UL == fixedpoint_frac_part(objs->zero));
  ASSERT(0UL == fixedpoint_frac_part(objs->one));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(objs->one_half));
  ASSERT(0x4000000000000000UL == fixedpoint_frac_part(objs->one_fourth));
  ASSERT(0xec9a1e2418UL == fixedpoint_frac_part(objs->large1));
  ASSERT(0x4d1a23c24fafUL == fixedpoint_frac_part(objs->large2));
}

void test_create_from_hex(TestObjs *objs) {
  (void) objs;

  Fixedpoint val1 = fixedpoint_create_from_hex("f6a5865.00f2");

  ASSERT(fixedpoint_is_valid(val1));

  ASSERT(0xf6a5865UL == fixedpoint_whole_part(val1));

  ASSERT(0x00f2000000000000UL == fixedpoint_frac_part(val1));
}

void test_format_as_hex(TestObjs *objs) {
  char *s;

  s = fixedpoint_format_as_hex(objs->zero);
  ASSERT(0 == strcmp(s, "0"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one);
  ASSERT(0 == strcmp(s, "1"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one_half);
  ASSERT(0 == strcmp(s, "0.8"));
  free(s);

  s = fixedpoint_format_as_hex(objs->one_fourth);
  ASSERT(0 == strcmp(s, "0.4"));
  free(s);

  s = fixedpoint_format_as_hex(objs->large1);
  ASSERT(0 == strcmp(s, "4b19efcea.000000ec9a1e2418"));
  free(s);

  s = fixedpoint_format_as_hex(objs->large2);
  ASSERT(0 == strcmp(s, "fcbf3d5.00004d1a23c24faf"));
  free(s);
}

void test_negate(TestObjs *objs) {
  // none of the test fixture objects are negative
  ASSERT(!fixedpoint_is_neg(objs->zero));
  ASSERT(!fixedpoint_is_neg(objs->one));
  ASSERT(!fixedpoint_is_neg(objs->one_half));
  ASSERT(!fixedpoint_is_neg(objs->one_fourth));
  ASSERT(!fixedpoint_is_neg(objs->large1));
  ASSERT(!fixedpoint_is_neg(objs->large2));

  // negate the test fixture values
  Fixedpoint zero_neg = fixedpoint_negate(objs->zero);
  Fixedpoint one_neg = fixedpoint_negate(objs->one);
  Fixedpoint one_half_neg = fixedpoint_negate(objs->one_half);
  Fixedpoint one_fourth_neg = fixedpoint_negate(objs->one_fourth);
  Fixedpoint large1_neg = fixedpoint_negate(objs->large1);
  Fixedpoint large2_neg = fixedpoint_negate(objs->large2);

  // zero does not become negative when negated
  ASSERT(!fixedpoint_is_neg(zero_neg));

  // all of the other values should have become negative when negated
  ASSERT(fixedpoint_is_neg(one_neg));
  ASSERT(fixedpoint_is_neg(one_half_neg));
  ASSERT(fixedpoint_is_neg(one_fourth_neg));
  ASSERT(fixedpoint_is_neg(large1_neg));
  ASSERT(fixedpoint_is_neg(large2_neg));

  // magnitudes should stay the same
  ASSERT(0UL == fixedpoint_whole_part(objs->zero));
  ASSERT(1UL == fixedpoint_whole_part(objs->one));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_half));
  ASSERT(0UL == fixedpoint_whole_part(objs->one_fourth));
  ASSERT(0x4b19efceaUL == fixedpoint_whole_part(objs->large1));
  ASSERT(0xfcbf3d5UL == fixedpoint_whole_part(objs->large2));
  ASSERT(0UL == fixedpoint_frac_part(objs->zero));
  ASSERT(0UL == fixedpoint_frac_part(objs->one));
  ASSERT(0x8000000000000000UL == fixedpoint_frac_part(objs->one_half));
  ASSERT(0x4000000000000000UL == fixedpoint_frac_part(objs->one_fourth));
  ASSERT(0xec9a1e2418UL == fixedpoint_frac_part(objs->large1));
  ASSERT(0x4d1a23c24fafUL == fixedpoint_frac_part(objs->large2));
}

void test_add(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;

  lhs = fixedpoint_create_from_hex("-c7252a193ae07.7a51de9ea0538c5");
  rhs = fixedpoint_create_from_hex("d09079.1e6d601");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT(0xc7252a0c31d8eUL == fixedpoint_whole_part(sum));
  ASSERT(0x5be47e8ea0538c50UL == fixedpoint_frac_part(sum));
}

void test_sub(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, diff;

  lhs = fixedpoint_create_from_hex("-ccf35aa3a04a3b.b105");
  rhs = fixedpoint_create_from_hex("f676e8.58");
  diff = fixedpoint_sub(lhs, rhs);
  ASSERT(fixedpoint_is_neg(diff));
  ASSERT(0xccf35aa496c124UL == fixedpoint_whole_part(diff));
  ASSERT(0x0905000000000000UL == fixedpoint_frac_part(diff));
}

void test_is_overflow_pos(TestObjs *objs) {
  Fixedpoint sum;

  sum = fixedpoint_add(objs->max, objs->one);
  ASSERT(fixedpoint_is_overflow_pos(sum));

  sum = fixedpoint_add(objs->one, objs->max);
  ASSERT(fixedpoint_is_overflow_pos(sum));

  Fixedpoint negative_one = fixedpoint_negate(objs->one);

  sum = fixedpoint_sub(objs->max, negative_one);
  ASSERT(fixedpoint_is_overflow_pos(sum));
}

void test_is_err(TestObjs *objs) {
  (void) objs;

  // too many characters
  Fixedpoint err1 = fixedpoint_create_from_hex("88888888888888889.6666666666666666");
  ASSERT(fixedpoint_is_err(err1));

  // too many characters
  Fixedpoint err2 = fixedpoint_create_from_hex("6666666666666666.88888888888888889");
  ASSERT(fixedpoint_is_err(err2));

  // this one is actually fine
  Fixedpoint err3 = fixedpoint_create_from_hex("-6666666666666666.8888888888888888");
  ASSERT(fixedpoint_is_valid(err3));
  ASSERT(!fixedpoint_is_err(err3));

  // whole part is too large
  Fixedpoint err4 = fixedpoint_create_from_hex("88888888888888889");
  ASSERT(fixedpoint_is_err(err4));

  // fractional part is too large
  Fixedpoint err5 = fixedpoint_create_from_hex("7.88888888888888889");
  ASSERT(fixedpoint_is_err(err5));

  // invalid hex digits in whole part
  Fixedpoint err6 = fixedpoint_create_from_hex("123xabc.4");
  ASSERT(fixedpoint_is_err(err6));

  // invalid hex digits in fractional part
  Fixedpoint err7 = fixedpoint_create_from_hex("7.0?4");
  ASSERT(fixedpoint_is_err(err7));
}

// TODO: implement more test functions
/*
MS 1 Test Funtions: 
fixedpoint_create // DONE (other fnc rely)
fixedpoint_create2 // DONE (other fnc rely)
fixedpoint_whole_part // DONE (given)
fixedpoint_frac_part // DONE (given)
fixedpoint_is_zero // DONE (given)

MS 2 Test Functions
fixedpoint_create_from_hex // DONE (have 2 tests)
fixedpoint_negate // given
fixedpoint_add // DONE (3 tests)
fixedpoint_sub // DONE (same as add)
fixedpoint_halve // DONE
fixedpoint_double // DONE
fixedpoint_compare // DONE
fixedpoint_is_err // DONE
fixedpoint_is_neg // contained in most other tests
fixedpoint_is_overflow_neg // given
fixedpoint_is_overflow_pos // given
fixedpoint_is_underflow_neg // tested in fnc test_halve
fixedpoint_is_underflow_pos // tested in fnc test_halve
fixedpoint_is_valid // tested multiple times
fixedpoint_format_as_hex // given
*/

void test_create_from_hex2(TestObjs *objs) {
  (void) objs;

  // Test positive
  Fixedpoint val1 = fixedpoint_create_from_hex("d09079.1e6d601");
  ASSERT(!fixedpoint_is_err(val1));
  ASSERT(fixedpoint_is_valid(val1));
  ASSERT(!fixedpoint_is_neg(val1));
  ASSERT(0xd09079 == fixedpoint_whole_part(val1));
  ASSERT(2192514215435042816 == fixedpoint_frac_part(val1));

  // Test negative
  Fixedpoint val2 = fixedpoint_create_from_hex("-000d09079A567709.0001e6d601");
  ASSERT(!fixedpoint_is_err(val2));
  ASSERT(fixedpoint_is_valid(val2));
  ASSERT(fixedpoint_is_neg(val2));
  ASSERT(0xd09079A567709UL == fixedpoint_whole_part(val2));
  ASSERT(0x0001e6d601000000UL == fixedpoint_frac_part(val2));


  Fixedpoint max = fixedpoint_create_from_hex("FFFFFFFFFFFFFFFF.FFFFFFFFFFFFFFFF");
  ASSERT(!fixedpoint_is_err(max));
  ASSERT(fixedpoint_is_valid(max));
  ASSERT(!fixedpoint_is_neg(max));
  ASSERT(0xFFFFFFFFFFFFFFFFUL == fixedpoint_whole_part(max));
  ASSERT(0xFFFFFFFFFFFFFFFFUL == fixedpoint_frac_part(max));


  // Test starts with 0
  Fixedpoint val3 = fixedpoint_create_from_hex("0.1e6d601");
  ASSERT(!fixedpoint_is_err(val3));
  ASSERT(fixedpoint_is_valid(val3));
  ASSERT(!fixedpoint_is_neg(val3));
  ASSERT(0 == fixedpoint_whole_part(val3));
  ASSERT(2192514215435042816 == fixedpoint_frac_part(val3));

  Fixedpoint zero = fixedpoint_create_from_hex("-000.0000");
  ASSERT(!fixedpoint_is_neg(zero));
  ASSERT(0 == fixedpoint_whole_part(zero));
  ASSERT(0 == fixedpoint_frac_part(zero));
}

void test_is_err2(TestObjs *objs) {
  (void) objs;
  // Starts with "."
  Fixedpoint valid1 = fixedpoint_create_from_hex(".6666666666666666");
  ASSERT(!fixedpoint_is_err(valid1));

  // "-" and starts with "."
  Fixedpoint valid2 = fixedpoint_create_from_hex("-.6666666666666666");
  ASSERT(!fixedpoint_is_err(valid2));

  // Has @
  Fixedpoint err1 = fixedpoint_create_from_hex("@.0");
  ASSERT(fixedpoint_is_err(err1));

  // Has G
  Fixedpoint err2 = fixedpoint_create_from_hex("G.0");
  ASSERT(fixedpoint_is_err(err2));

  
}


void test_add2(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;
  lhs = fixedpoint_create_from_hex("-c7252a193ae07.7a51de9ea0538c5");
  rhs = fixedpoint_create_from_hex("d09079.1e6d601");
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(fixedpoint_is_neg(sum));
  ASSERT((3503398930554254) == fixedpoint_whole_part(sum));
  ASSERT((6621556503181757520) == fixedpoint_frac_part(sum));
}
void test_add3(TestObjs *objs) {
  (void) objs;

  Fixedpoint lhs, rhs, sum;

  lhs = fixedpoint_create_from_hex("1.8000000000000000");
  rhs = lhs;
  sum = fixedpoint_add(lhs, rhs);
  ASSERT(!fixedpoint_is_neg(sum));
  ASSERT(3 == fixedpoint_whole_part(sum));
  ASSERT(0 == fixedpoint_frac_part(sum));
}

void test_halve(TestObjs *objs) {
  (void) objs;
  Fixedpoint large2_neg = fixedpoint_negate(objs->large2);

  Fixedpoint positive_underflow = fixedpoint_halve(objs->large2);
  Fixedpoint negative_underflow = fixedpoint_halve(large2_neg);
  Fixedpoint half = fixedpoint_halve(objs->large1);
  Fixedpoint zero = fixedpoint_halve(objs->zero);
  Fixedpoint max_even_half = fixedpoint_halve(fixedpoint_sub(objs->max, objs->min));
  
  ASSERT(fixedpoint_is_underflow_pos(positive_underflow));
  ASSERT(fixedpoint_is_underflow_neg(negative_underflow));
  ASSERT( 0x258cf7e75UL == fixedpoint_whole_part(half));
  ASSERT(0x764d0f120cUL == fixedpoint_frac_part(half));
  ASSERT(0x7fffffffffffffffUL == fixedpoint_whole_part(max_even_half));
  ASSERT(0xffffffffffffffffUL == fixedpoint_frac_part(max_even_half));
  ASSERT(0 == fixedpoint_whole_part(zero));
  ASSERT(0 == fixedpoint_frac_part(zero));
}

void test_double(TestObjs *objs) {
  (void) objs;

  Fixedpoint max_neg = fixedpoint_negate(objs->max);
  Fixedpoint positive_overflow = fixedpoint_double(objs->max);
  Fixedpoint negative_overflow = fixedpoint_double(max_neg);
  Fixedpoint double_large1 = fixedpoint_double(objs->large1);
  Fixedpoint double_large3 = fixedpoint_double(objs->large3);
  Fixedpoint zero = fixedpoint_double(objs->zero);
  
  ASSERT(fixedpoint_is_overflow_pos(positive_overflow));
  ASSERT(fixedpoint_is_overflow_neg(negative_overflow));

  ASSERT(0x9633df9d4UL == fixedpoint_whole_part(double_large1));
  ASSERT(0x1d9343c4830UL == fixedpoint_frac_part(double_large1));
  ASSERT(0x7f97e7abUL == fixedpoint_whole_part(double_large3));

  ASSERT(0xda34478f3f400000UL == fixedpoint_frac_part(double_large3));
  ASSERT(0 == fixedpoint_whole_part(zero));
  ASSERT(0 == fixedpoint_frac_part(zero));
}

void test_is_overflow_pos2(TestObjs *objs) {
  Fixedpoint sum;

  sum = fixedpoint_add(objs->max, objs->max);
  ASSERT(fixedpoint_is_overflow_pos(sum));

  sum = fixedpoint_add(objs->half_max, objs->half_max);
  ASSERT(fixedpoint_is_overflow_pos(sum));

  sum = fixedpoint_add(objs->half_max_almost, objs->half_max_almost);
  ASSERT(!fixedpoint_is_overflow_pos(sum));
}

void test_compare(TestObjs *objs) {
  // Test close values

  ASSERT(1 == fixedpoint_compare(objs->half_max, objs->half_max_almost));
  ASSERT(0 == fixedpoint_compare(objs->half_max, objs->half_max));
  ASSERT(-1 == fixedpoint_compare(objs->half_max_almost, objs->half_max));

  Fixedpoint neg_half_max = fixedpoint_negate(objs->half_max);

  ASSERT(-1 == fixedpoint_compare(neg_half_max, objs->half_max_almost));
  ASSERT(0 == fixedpoint_compare(neg_half_max, neg_half_max));
  ASSERT(1 == fixedpoint_compare(objs->half_max_almost, neg_half_max));

}

void test_is_valid(TestObjs *objs) {
  // Overflow
  Fixedpoint positive_overflow = fixedpoint_double(objs->max);
  Fixedpoint neg_max = fixedpoint_negate(objs->max);
  Fixedpoint negative_overflow = fixedpoint_double(neg_max);
  ASSERT(fixedpoint_is_overflow_pos(positive_overflow));
  ASSERT(fixedpoint_is_overflow_neg(negative_overflow));
  ASSERT(!fixedpoint_is_valid(positive_overflow));
  ASSERT(!fixedpoint_is_valid(negative_overflow));


  // Underflow
  Fixedpoint positive_underflow = fixedpoint_halve(objs->large2);
  Fixedpoint neg_large2 = fixedpoint_negate(objs->large2);
  Fixedpoint negative_underflow = fixedpoint_halve(neg_large2);
  ASSERT(fixedpoint_is_underflow_pos(positive_underflow));
  ASSERT(fixedpoint_is_underflow_neg(negative_underflow));
  ASSERT(!fixedpoint_is_valid(positive_underflow));
  ASSERT(!fixedpoint_is_valid(negative_underflow));


  // is_err
  Fixedpoint err = fixedpoint_create_from_hex(".66666.66666666666");
  ASSERT(fixedpoint_is_err(err));
  ASSERT(!fixedpoint_is_valid(err));
}

// Test randomly generated numbers from provided Ruby script
void test_rby(TestObjs *objs) {
  Fixedpoint sum = fixedpoint_add(objs->rand1, objs->rand2);
  ASSERT(0x1b3f57521b20af == fixedpoint_whole_part(sum));
  ASSERT(0x00466e8665e00000 == fixedpoint_frac_part(sum));
  ASSERT(!fixedpoint_is_neg(sum));
  ASSERT(!fixedpoint_is_err(sum));
  ASSERT(fixedpoint_is_valid(sum));

  Fixedpoint diff = fixedpoint_sub(objs->rand3, objs->rand4);
  ASSERT(0x1c12ab850627 == fixedpoint_whole_part(diff));
  ASSERT(0x8528fadb45e52f00 == fixedpoint_frac_part(diff));
  ASSERT(!fixedpoint_is_neg(diff));
  ASSERT(!fixedpoint_is_err(diff));
  ASSERT(fixedpoint_is_valid(diff));
}