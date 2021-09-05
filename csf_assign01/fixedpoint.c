#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "fixedpoint.h"
#include <math.h>

// You can remove this once all of the functions are fully implemented
static Fixedpoint DUMMY;

Fixedpoint fixedpoint_create(uint64_t whole) {
  
  Fixedpoint newFP;
  newFP.whole = whole;
  newFP.frac = 0;

  return newFP;
}

Fixedpoint fixedpoint_create2(uint64_t whole, uint64_t frac) {
  
  Fixedpoint newFP;
  newFP.whole = whole;
  newFP.frac = frac;
  return newFP;
}

Fixedpoint fixedpoint_create_from_hex(const char *hex) {
  Fixedpoint newFP;
  newFP.whole = 0;
  newFP.frac = 0;
  int index = 0;
  int is_fraction = 0;
  int count = 16;

  if(hex[index] == '-'){
    newFP.is_negative = 1;
    index++;
  }
  else if(!(isdigit(hex[index]) || (96 < hex[index] && hex[index] < 103) || ( 64 < hex[index] && hex[index] < 71))){
    newFP.invalid = 1;
    return newFP;
  }

  while(index < (int)strlen(hex)){
    if(hex[index] == '.'){
      is_fraction = 1;
      index++;
    }
    else if( isdigit(hex[index]) || (96 < hex[index] && hex[index] < 103) || ( 64 < hex[index] && hex[index] < 71)){
      if(is_fraction){
        newFP.frac *= 16;
        char temp[2] = {'\0', '\0'};
        temp[0] = hex[index];
        
        newFP.frac += strtoul(temp, NULL, 16);
        count--;
      }
      else{
        newFP.whole *= 16;
        char temp[2] = {'\0', '\0'};
        temp[0] = hex[index]; 
        newFP.whole += strtoul(temp, NULL, 16);
      }
      index++;
    }
    else{
      newFP.invalid = 1;
      return newFP;
    }
  }

  newFP.frac *= pow(16, count);
  return newFP;
  
}

uint64_t fixedpoint_whole_part(Fixedpoint val) {
  return val.whole;
}

uint64_t fixedpoint_frac_part(Fixedpoint val) {
  return val.frac;
}

uint64_t frac_pos_overflow (uint64_t left, uint64_t right) {
  // Check
  uint64_t n = sizeof(uint8_t) * 8;
  // Set 64th bit of frac to 0;
  uint64_t l_63 = left &= ~(1 << n);
  uint64_t r_63 = right &= ~(1 << n);
  uint64_t frac_sum_63 = l_63 + r_63;

  if (frac_sum_63 >> n - 1) { // if last 63 bits add to 64th bit having 1
    if (left >> 63 && right >> 63) {
      return 2; // If 64th bit frac_sum_63 is 1 and 64th bit left and right = 1, 64th bit of sum = 1 and add 1 to whole
    } else if ((left >> 63) ^ (right >> 63)) {
      return 1; // 64th bit of sum = 0 and add 1 to whole
    } else {
      return 0; // no overflow
    } 
  } else { // if last 63 bits do not add to 1
    if (left >> 63 && right >> 63) { // 
      return 1; //64th bit of sum = 0 and add 1 to whole
    } else {
      return 0; // no overflow
    }
  }
}

Fixedpoint fixedpoint_add(Fixedpoint left, Fixedpoint right) {
  // Psuedocode: Must check for +overflow or -overflow
  // +Overflow: Sum the 63 lowest bits. If the result has the 64 bit being 1,
  //            then if either left or right has a 1 in the 64th place, then
  //            + overflow occurs.
  // -Overflow: Same process, but only occurs for two negative.
  // If fractional part overflows, then must add 1 to whole num;

  /*

  Fixedpoint sum;
  if (left.is_negative == 0) {
    if (right.is_negative == 0) { // +left + right. Possible +overflow
    uint64_t frac_overflow = frac_pos_overflow(left.frac, right.frac);
      if (frac_overflow == 2) {
        sum.frac = ((left.frac &= ~(1 << n)) + (right.frac &= ~(1 << n))) |= (1 << 64);
        if (!frac_pos_overflow(left.whole, right.whole)) {
          uint64_t sum.whole = left.whole + right.whole;
          if (!frac_pos_overflow(sum.whole, 1) {
            uint64_t sum.whole = left.whole + right.whole;
            return sum;
          }
        } else {
          sum.is_overflow = 1;
          sum.is_negative = 0;
          return sum;
        }
      } else {
        sum.whole = left.whole + right.whole;
        sum.frac = left.frac + right.frac;
      }
    } else { // +left - right. Possible to have negative number
      if (left.whole )

    }
  }

  return DUMMY;
  */
}

Fixedpoint fixedpoint_sub(Fixedpoint left, Fixedpoint right) {
  Fixedpoint right_negate = fixedpoint_negate(right);
  Fixedpoint diff = fixedpoint_add(left, right_negate);

  return diff;
}

Fixedpoint fixedpoint_negate(Fixedpoint val) {
  val.is_negative = !val.is_negative;
  return val;
}

Fixedpoint fixedpoint_halve(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return DUMMY;
}

Fixedpoint fixedpoint_double(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return DUMMY;
}

int fixedpoint_compare(Fixedpoint left, Fixedpoint right) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_zero(Fixedpoint val) {
  if(val.whole == 0 && val.frac == 0){
    return 1;
  }

  return 0;
  
}

int fixedpoint_is_err(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_neg(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_overflow_neg(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_overflow_pos(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_underflow_neg(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_underflow_pos(Fixedpoint val) {
  // TODO: implement
  assert(0);
  return 0;
}

int fixedpoint_is_valid(Fixedpoint val) {
  // TODO: implement
  return !val.invalid;
}

char *fixedpoint_format_as_hex(Fixedpoint val) {
  // TODO: implement
  assert(0);
  char *s = malloc(20);
  strcpy(s, "<invalid>");
  return s;
}
