/*
 * Implrementation of functions declared in textsearch_fns.h for finding # occurrences
 * CSF Assignment 2
 * Aidan Aug, Will Zhao
 * aaug1@jhu.edu, wzhao33@jhu.edu
 */


#include <stdio.h>
#include "textsearch_fns.h"

// TODO: implement functions declared in textsearch_fns.h
int read_line(FILE *in, char *buf) {

	// Read in the first character
	char c = fgetc(in);
	int count = 0;

	// Loop over all characters in the line
	while (c != '\n' && c != EOF && count < MAXLINE) {
		buf[count] = c;
		c = fgetc(in);
		count++;
	}
	buf[count] = '\0';

	// Once EOF is reached, return 1
	if (c == EOF) {
		return 0;
	}
	
	return 1;
}

void print_line(FILE *out, const char *buf) {
	fprintf(out, buf);
}

unsigned count_occurrences(const char *line, const char *str) {
	int occurrences = 0;
	int line_len = find_string_length(line);
	int search_word_len = find_string_length(str);

	// If line length is less than search word length, then no occurrences possible
	if (line_len < search_word_len) {
		return 0;
	}

	// Iterate over the entire line
	for (int i = 0; i < line_len - search_word_len; i++) {
		occurrences += strings_equal(line + i, str);
	}

	return occurrences;
}


unsigned find_string_length(const char *s) {
	int tot_len = 0;
	char c = *s;

	while(c != '\0') {
		c = *(s+tot_len);
		tot_len++;
	}

	return tot_len - 1;
}


int strings_equal(const char *s1, const char *s2) {
	int matches = 0;
	int search_word_len = find_string_length(s2);

	// Iterate over the length of the searched word.
	for (int j = 0; j < search_word_len; j++) {
		if (s1[j] == s2[j]) {
			matches++;
		}
	}

	// If we got the same number of matched characters
	if (matches == search_word_len) {
		return 1;
	}
	
	return 0;
}

int handle_arguments(int argc) {
	
  if (argc == 3) {
    return 2;    
  } else if (argc == 4) {
    return 3;
  } else {
    fprintf(stderr, "Invalid input");
	
    exit(1);
  }

}

int calc_total_occurences(FILE* fileptr, char* search, int argc) {
  int flag = 1;
  int tot_occurrences;
  int num_occurrences;
  while (flag) {
    char* buff = malloc(sizeof(char) * 512);
    flag = read_line(fileptr, buff);
    num_occurrences = count_occurrences(buff, search);
    tot_occurrences += num_occurrences;
    if (argc == 3 && num_occurrences > 0) {
      fprintf(stdout, buff);
      fprintf(stdout, "\n");
    }
  }

  return tot_occurrences;
}
