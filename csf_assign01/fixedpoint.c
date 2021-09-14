/*
 * Fixedpoint Numeric Data Type and Arithmetic: Implementation
 * CSF Assignment 1
 * Aidan Aug, Will Zhao
 * aaug1@jhu.edu, wzhao33@jhu.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "fixedpoint.h"
#include <math.h>

Fixedpoint fixedpoint_create(uint64_t whole) {
  // Initialize all fields
  Fixedpoint new_fp;
  new_fp.whole = whole;
  new_fp.frac = 0;
  new_fp.is_negative = 0;
  new_fp.is_err = 0;
  new_fp.is_underflow = 0;
  new_fp.is_overflow = 0;

  return new_fp;
}

Fixedpoint fixedpoint_create2(uint64_t whole, uint64_t frac) {
  // Initialize all fields
  Fixedpoint new_fp;
  new_fp.whole = whole;
  new_fp.frac = frac;
  new_fp.is_negative = 0;
  new_fp.is_err = 0;
  new_fp.is_underflow = 0;
  new_fp.is_overflow = 0;
  return new_fp;
}

Fixedpoint fixedpoint_create_from_hex(const char *hex) {
  Fixedpoint new_fp = fixedpoint_create(0);

  int index = 0;
  int is_fraction = 0;
  int count = 16;

  // Check negative
  if(hex[index] == '-') {
    new_fp.is_negative = 1;
    index++;
  } 
  
  // Loop over entire string
  int whole_len_counter = 1;
  int frac_len_counter = 0;
  while(index < (int)strlen(hex)){   

    // Check to see if decimal point was reached 
    if(hex[index] == '.'){
      if(is_fraction == 1) {
        new_fp.is_err = 1;
        return new_fp;
      }
      is_fraction = 1;
      frac_len_counter = 1;
      index++;
    } else if(isdigit(hex[index]) || (96 < hex[index] && hex[index] < 103) || ( 64 < hex[index] && hex[index] < 71)){
      if(is_fraction){
        // Check length of fractional component
        if(frac_len_counter > 16) {
          new_fp.is_err = 1;
          return new_fp;
        }
        // Calculate fractional representation
        new_fp.frac *= 16;
        char temp[2] = {'\0', '\0'};
        temp[0] = hex[index];
        new_fp.frac += strtoul(temp, NULL, 16);
        frac_len_counter++;
        count--;
      } else {
        // Check length of whole component
        if(whole_len_counter > 16) {
          new_fp.is_err = 1;
          return new_fp;
        }
        // Calculate whole representation
        new_fp.whole *= 16;
        char temp[2] = {'\0', '\0'};
        temp[0] = hex[index]; 
        new_fp.whole += strtoul(temp, NULL, 16);
        whole_len_counter++;
      }
      index++;
    } 
    else {
      new_fp.is_err = 1;
      return new_fp;
    }
  }

  // Make sure representation zero pads
  for(int i = 0; i< count; i++){
    new_fp.frac *= 16;
  }

  // If zero, then should not be negative
  if(new_fp.frac == 0 && new_fp.whole == 0){
    new_fp.is_negative = 0;
  }

  return new_fp;
}

uint64_t fixedpoint_whole_part(Fixedpoint val) {
  return val.whole;
}

uint64_t fixedpoint_frac_part(Fixedpoint val) {
  return val.frac;
}

// Computes the sum of a positive and negative number
//
// Parameters:
//   pos - the positive Fixedpoint value
//   neg - the negative Fixedpoint value
//
// Returns:
//     the Fixedpoint value for the sum of a positive
//     and negative number
Fixedpoint add_diff_sign(Fixedpoint pos, Fixedpoint neg) {
  Fixedpoint sum = fixedpoint_create(0);

  // Determine which has the greater magnitude for calculation for whole
  if (pos.whole > neg.whole) {
    sum.is_negative = 0;
    // Determine which has the greater magnitude for frac and perform respective calculation
    if (pos.frac >= neg.frac) {
      sum.whole = pos.whole - neg.whole;
      sum.frac = pos.frac - neg.frac;
    } else {
      sum.whole = pos.whole - neg.whole - 1;
      sum.frac = 0xFFFFFFFFFFFFFFFF - neg.frac;
      sum.frac +=  (pos.frac + 1);
    }
  } else if (pos.whole < neg.whole) {
    sum.is_negative = 1;
    if (pos.frac <= neg.frac) {
      sum.whole = neg.whole - pos.whole;
      sum.frac = neg.frac - pos.frac;
    } else {
      sum.whole = neg.whole - pos.whole - 1;
      sum.frac = 0xFFFFFFFFFFFFFFFF - pos.frac;
      sum.frac += (neg.frac + 1);
    }
  } else { // Accounts for case where equal
    if (pos.frac >= neg.frac){
      sum.whole = pos.whole - neg.whole;
      sum.frac = pos.frac - neg.frac;
    } else {
      sum.whole = pos.whole - neg.whole;
      sum.frac = neg.frac - pos.frac;
      sum.is_negative = 1;
    }
  }
  return sum;
}

// Computes the mag of two same-signed numbers
//
// Parameters:
//   left - the left Fixedpoint value
//   right - the right Fixedpoint value
//
// Returns:
//     the Fixedpoint value with the magnitude of
//     the sum for two same-signed numbers
Fixedpoint add_same_sign(Fixedpoint left, Fixedpoint right) {
  // Note: If both + or both -, have to check overflow
  Fixedpoint sum = fixedpoint_create(0);
  sum.is_negative = left.is_negative;
  uint64_t whole = (left.whole + right.whole);

  if (left.whole > whole || right.whole > whole) {// This means overflow occurred
    sum.is_overflow = 1;
    return sum;
  }

  sum.frac = (left.frac + right.frac);

  if (left.frac > sum.frac || right.frac > sum.frac) {// This means overflow occurred in frac so add 1 to whole
    if (whole == 0xffffffffffffffffLU) {
      sum.is_overflow = 1;
      return sum;
    } else {
      whole = whole + 1;
    }
  }

  sum.whole = whole;
  
  return sum;
}

Fixedpoint fixedpoint_add(Fixedpoint left, Fixedpoint right) {
  // Account for overflow, underflow, negative
  Fixedpoint sum = fixedpoint_create(0);

  if (left.is_negative == 0 && right.is_negative == 1) {
    sum = add_diff_sign(left, right);
  } else if (left.is_negative == 1 && right.is_negative == 0) {
    sum = add_diff_sign(right, left);
  } else {
    sum = add_same_sign(left, right);
  }
  return sum;
}

Fixedpoint fixedpoint_sub(Fixedpoint left, Fixedpoint right) {
  // Same as add just negate the right value

  Fixedpoint right_negate = fixedpoint_negate(right);
  Fixedpoint diff = fixedpoint_add(left, right_negate);
  return diff;
}

Fixedpoint fixedpoint_negate(Fixedpoint val) {
  if (fixedpoint_is_zero(val)) {
    return val;
  } else if (val.is_negative == 0) {
    val.is_negative = 1;
  } else {
    val.is_negative = 0;
  }

  return val;
}

Fixedpoint fixedpoint_halve(Fixedpoint val) {

  Fixedpoint half = fixedpoint_create(0);
  half.is_negative = val.is_negative;

  // Check underflow
  if (val.frac % 2 != 0) { 
    val.is_underflow = 1;
    return val;
  }

  uint64_t whole_frac = 0;
  half.whole = val.whole / 2;

  // If whole is an odd number, add 1/2 * 2^64 to frac
  if (val.whole % 2 != 0) {
    whole_frac = 0x8000000000000000;
  }
  
  half.frac = val.frac / 2 + whole_frac;
  return half;
}

Fixedpoint fixedpoint_double(Fixedpoint val) {
  // Double is the same as adding the same number twice
  Fixedpoint doubled = fixedpoint_add(val, val);
  return doubled;
}

int fixedpoint_compare(Fixedpoint left, Fixedpoint right) {
  int compare = 0;;
  // If both are zero
  if (fixedpoint_is_zero(left) == 1 && fixedpoint_is_zero(right) == 1) {
    return compare;
  }

  // Check sign of each Fixedpoint
  if ((left.is_negative == 0 || fixedpoint_is_zero(left)) && right.is_negative == 1) {
    compare = 1;
  } else if (left.is_negative == 1 && (right.is_negative == 0 || fixedpoint_is_zero(right))) {
    compare = -1;
  } else if (left.is_negative == right.is_negative) { // If same sign
    if (left.whole > right.whole) {
      compare = 1;
    } else if (left.whole < right.whole) {
      compare = -1;
    } else { // whole values are equal
      if (left.frac > right.frac) {
        compare = 1;
      } else if (left.frac < right.frac) {
        compare = -1;
      } else {
        compare = 0;
      }
    }
  }

  // If they were both negative, negate the compare value
  if (left.is_negative == 1 && right.is_negative == 1) {
    compare *= -1;
  }
  return compare;
}

int fixedpoint_is_zero(Fixedpoint val) {
  if(val.whole == 0 && val.frac == 0){
    return 1;
  }
  return 0;
  
}

int fixedpoint_is_err(Fixedpoint val) {
  return val.is_err;
}

int fixedpoint_is_neg(Fixedpoint val) {
  return val.is_negative;
}

int fixedpoint_is_overflow_neg(Fixedpoint val) {
  return val.is_overflow && val.is_negative;
}

int fixedpoint_is_overflow_pos(Fixedpoint val) {
  return val.is_overflow && !val.is_negative;
}

int fixedpoint_is_underflow_neg(Fixedpoint val) {
  return val.is_underflow && val.is_negative;
}

int fixedpoint_is_underflow_pos(Fixedpoint val) {
  return val.is_underflow && !val.is_negative;
}

int fixedpoint_is_valid(Fixedpoint val) {
  if (val.is_err || val.is_overflow || val.is_underflow) {
    return 0;
  } else {
    return 1;
  }
}

char *fixedpoint_format_as_hex(Fixedpoint val) {
  char *s = malloc(35);
  char reverse[35];
  int index = 0;
  uint64_t whole = val.whole;
  uint64_t frac = val.frac;

  if (frac != 0){
    int count = 0;

    while(frac % 16 == 0){
      frac /= 16;
      count++;
    }

    while (frac != 0){
      int digit = frac % 16;
      if (digit < 10){
      reverse[index] = digit + '0';
      } else {
        reverse[index] = digit + 'W';
      }

    frac /= 16;
    index++;
    count++;

    }

    for(int i = 16; i > count; i--){
      reverse[index] = '0';
      index++;
    }

    reverse[index] = '.';
    index++;
  }

  if (whole == 0){
    reverse[index] = '0';
    index++;
  }

  while (whole != 0){
    int digit = whole % 16;
    if (digit < 10){
      reverse[index] = digit + '0';
    } else {
      reverse[index] = digit + 'W';
    }
    whole /= 16;
    index++;
  }

  if (val.is_negative){
    reverse[index] = '-';
    index++;
  }

  for (int i = 0; i < index; i++){
    s[i] = reverse[index - i - 1];
  }

  s[index] = '\0';
  return s;
}
