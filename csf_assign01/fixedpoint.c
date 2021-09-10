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

// You can remove this once all of the functions are fully implemented
static Fixedpoint DUMMY;

Fixedpoint fixedpoint_create(uint64_t whole) {
  
  Fixedpoint new_fp;
  new_fp.whole = whole;
  new_fp.frac = 0;
  new_fp.is_negative = 0;
  new_fp.invalid = 0;
  new_fp.is_underflow = 0;
  new_fp.is_overflow = 0;

  return new_fp;
}

Fixedpoint fixedpoint_create2(uint64_t whole, uint64_t frac) {
  
  Fixedpoint new_fp;
  new_fp.whole = whole;
  new_fp.frac = frac;
  new_fp.is_negative = 0;
  new_fp.invalid = 0;
  new_fp.is_underflow = 0;
  new_fp.is_overflow = 0;
  return new_fp;
}

Fixedpoint fixedpoint_create_from_hex(const char *hex) {
  Fixedpoint new_fp;
  new_fp.whole = 0;
  new_fp.frac = 0;
  int index = 0;
  int is_fraction = 0;
  int count = 16;

  if(hex[index] == '-'){
    new_fp.is_negative = 1;
    index++;
  }
  else if(!(isdigit(hex[index]) || (96 < hex[index] && hex[index] < 103) || ( 64 < hex[index] && hex[index] < 71))){
    new_fp.invalid = 1;
    return new_fp;
  }

  while(index < (int)strlen(hex)){
    if(hex[index] == '.'){
      is_fraction = 1;
      index++;
    }
    else if( isdigit(hex[index]) || (96 < hex[index] && hex[index] < 103) || ( 64 < hex[index] && hex[index] < 71)){
      if(is_fraction){
        new_fp.frac *= 16;
        char temp[2] = {'\0', '\0'};
        temp[0] = hex[index];
        
        new_fp.frac += strtoul(temp, NULL, 16);
        count--;
      }
      else{
        new_fp.whole *= 16;
        char temp[2] = {'\0', '\0'};
        temp[0] = hex[index]; 
        new_fp.whole += strtoul(temp, NULL, 16);
      }
      index++;
    }
    else{
      new_fp.invalid = 1;
      return new_fp;
    }
  }
  for(int i = 0; i< count; i++){
    new_fp.frac *= 16;
  }
  return new_fp;
  
}

uint64_t fixedpoint_whole_part(Fixedpoint val) {
  return val.whole;
}

uint64_t fixedpoint_frac_part(Fixedpoint val) {
  return val.frac;
}

Fixedpoint frac_overflow(Fixedpoint left, Fixedpoint right) {
  Fixedpoint sum_frac = fixedpoint_create(0);

  // If one is negative, no overflow is possible. Return sum/diff
  uint64_t n = sizeof(uint64_t) * 8;

  if (left.is_negative == 0 && right.is_negative == 1) {
    if (left.whole > right.whole) {
      if (left.frac > right.frac) {
        sum_frac.frac = left.frac - right.frac;
      } else {
        sum_frac.frac = right.frac - left.frac;
        sum_frac.is_negative = 1;
      }
    } else if (left.whole < right.whole) {
      if (left.frac > right.frac) {
        sum_frac.frac = left.frac - right.frac;
      } else {
        sum_frac.frac = right.frac - left.frac;
        sum_frac.is_negative = 1;
      }
    }
  } else if (right.is_negative == 0 && left.is_negative == 1) {
    if (right.whole > left.whole) {
      if (right.frac > left.frac) {
        sum_frac.frac = right.frac - left.frac;
      } else {
        sum_frac.frac = left.frac - right.frac;
        sum_frac.is_negative = 1;
      }
    } else if (right.whole < left.whole) {
      if (right.frac > left.frac) {
        sum_frac.frac = right.frac - left.frac;
      } else {
        sum_frac.frac = left.frac - right.frac;
        sum_frac.is_negative = 1;
      }
    }
    return sum_frac;
  }

  /*
  if (left.is_negative != right.is_negative) {
    if (left.frac > right.frac) {
      sum_frac.frac = left.frac - right.frac;
      if (left.is_negative) {
        sum_frac.is_negative = 1;
      }
    } else {
      sum_frac.frac = right.frac - left.frac;
      if (right.is_negative) {
        sum_frac.is_negative = 1;
      }
    }
    return sum_frac;
  }
  */

  // If both + or both -, overflow can occur
  if (left.is_negative && right.is_negative) {
    sum_frac.is_negative = 1;
  }
  
  // If overflow occurs, the resulting magnitude will be less than both of the other magnitudes
  uint64_t eff_sum = (left.frac + right.frac) % ((uint64_t) pow(2, 63));
  sum_frac.frac = eff_sum; // Effective sum

  uint64_t summed_frac = left.frac + right.frac;
  if (left.frac > summed_frac && right.frac > summed_frac) {// This means overflow occurred
    sum_frac.whole = 1;
  }

  return sum_frac;

}

Fixedpoint whole_overflow(Fixedpoint left, Fixedpoint right) {
  Fixedpoint sum_whole = fixedpoint_create(0);

  // If either is negative but not both, no possible overflow
  if (left.is_negative != right.is_negative) {
    if (left.whole > right.whole) {
      sum_whole.whole = left.whole - right.whole;
      if (left.is_negative) {
        sum_whole.is_negative = 1;
      }
    } else {
      sum_whole.whole = right.whole - left.whole;
      if (right.is_negative) {
        sum_whole.is_negative = 1;
      }
    }
    return sum_whole;
  }

  // If both + or both -, have to check overflow
  uint64_t n = sizeof(uint64_t) * 8;

  if (left.is_negative && right.is_negative) {
    sum_whole.is_negative = 1;
  }

  sum_whole.whole = left.whole + right.whole;
  if (left.whole > sum_whole.whole && right.whole > sum_whole.whole) {// This means overflow occurred
    sum_whole.is_overflow = 1;
  }

  return sum_whole;
}

Fixedpoint fixedpoint_add(Fixedpoint left, Fixedpoint right) {
  // Account for overflow, underflow, negative
  // First, calculate the sum of the fractional parts
  Fixedpoint sum_frac = frac_overflow(left, right);
  // Then, calculate the sum of the whole parts
  Fixedpoint sum = whole_overflow(left, right);
  sum = whole_overflow(sum, sum_frac);
  sum.frac = sum_frac.frac;
  return sum;
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
  
  return 0;
}

int fixedpoint_is_neg(Fixedpoint val) {
  // TODO: implement
  
  return val.is_negative;
}

int fixedpoint_is_overflow_neg(Fixedpoint val) {
  // TODO: implement
  
  return 0;
}

int fixedpoint_is_overflow_pos(Fixedpoint val) {
  // TODO: implement
  
  return 0;
}

int fixedpoint_is_underflow_neg(Fixedpoint val) {
  // TODO: implement
  
  return 0;
}

int fixedpoint_is_underflow_pos(Fixedpoint val) {
  // TODO: implement
  
  return 0;
}

int fixedpoint_is_valid(Fixedpoint val) {
  // TODO: implement
  return !val.invalid;
}

char *fixedpoint_format_as_hex(Fixedpoint val) {
  // TODO: implement
  
  char *s = malloc(20);
  strcpy(s, "<invalid>");
  return s;
}
