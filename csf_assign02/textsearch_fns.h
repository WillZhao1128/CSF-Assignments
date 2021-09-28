/*
 * Declaration of helper functions used for c_textsearch.c asm_textsearch.S
 * CSF Assignment 2
 * Aidan Aug, Will Zhao
 * aaug1@jhu.edu, wzhao33@jhu.edu
 */


#ifndef TEXTSEARCH_FNS_H
#define TEXTSEARCH_FNS_H

#define MAXLINE 511

// TODO: declare helper functions here (to be implemented in
// c_textsearch_fns.c and asm_textsearch_fns.S)

/*
 * Reads a single line from the file pointed to by "in"
 *
 * Parameters:
 * 	 in - pointer to file
 *   s - pointer to a C character string
 *
 * Returns:
 *   1 if end of file is not reached; 0 if end of file is reached
 */
int read_line(FILE *in, char *buf);

/*
 * Prints out the string pointed to by buf
 *
 * Parameters:
 * 	 out - pointer to file to write the string to
 *   buf - pointer to a C character string to be printed
 *
 * Returns:
 *   N/A
 */
void print_line(FILE *out, const char *buf);

/*
 * Counts the number of occurrences of a word str from line
 *
 * Parameters:
 * 	 line - pointer to a line (string) in a file
 *   str - pointer to a C character string to be printed
 *
 * Returns:
 *   number of occurrences
 */
unsigned count_occurrences(const char *line, const char *str);

/*
 * Determines the length of the string pointed to by s
 *
 * Parameters:
 *   s - pointer to a C string
 *
 * Returns:
 *   length of the string s
 */
unsigned find_string_length(const char *s);

/*
 * Determines if two strings are equivalent
 *
 * Parameters:
 *   s1 - pointer to a C character string (multiple words)
 *   s2 - pointer to a C character string (searched word)
 * 
 * Returns:
 *   1 if the strings are equal, 0 if the strings are not
 */
int strings_equal(const char *s1, const char *s2);

/*
 * Determines whether or not a valid command line argument pattern
 * was inputted
 *
 * Parameters:
 *   argc - number of command line arguments
 * 
 * Returns:
 *   integer location expected for .txt file
 */
int handle_arguments(int argc);

int calc_total_occurrences(FILE* fileptr, char* search, int argc);

#endif // TEXTSEARCH_FNS_H
