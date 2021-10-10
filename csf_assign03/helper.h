/*
 * Declaration of helper functions for use in main
 * CSF Assignment 3
 * Aidan Aug, Will Zhao
 * aaug1@jhu.edu, wzhao33@jhu.edu
 */

#ifndef HELPER_H
#define HELPER_H

/*
 * Determines if the passed string is a number that works for any
 * of the first three arguments
 *
 * Parameters:
 *   arg - a string argument
 *
 * Returns:
 *   0 if the provided string is either not a number, not a power of 2, or non-positive
 *   1 otherwise
 */
bool is_valid_number(char* arg);

/*
 * Handles all of the possible arguments
 *
 * Parameters:
 *   argv - an array of strings
 *
 * Returns:
 *   0 if the provided string array is invalid (see below)
 *   1 if the provided string is a valid sequence of strings
 */
bool handle_all_arguments(char* argv[]);

/*
 * Handles the first three provided command line arguments
 *
 * Parameters:
 *   argv - an array of strings
 *
 * Returns:
 *   0 if the provided strings are either not a number, not a power of 2, or non-positive.
 *   also returns 0 if the third argument is less than 4.
 *   1 if the first three command-line arguments are valid
 */
bool handle_number_arguments(char* argv[]);

/*
 * Handles the first three provided command line arguments
 *
 * Parameters:
 *   argv - an array of strings
 *
 * Returns:
 *   0 if the provided strings (last 3 command line arguments) are invalid (i.e., do not match
 *   one of the expected string inputs))
 *   1 if the last three command-line arguments are valid (i.e., match one of the expected string inputs)
 */
bool handle_string_arguments(char*argv[]);

#endif