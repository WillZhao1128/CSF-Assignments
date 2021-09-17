#include <stdio.h>
#include "textsearch_fns.h"

// TODO: implement functions declared in textsearch_fns.h
int read_line(FILE *in, char *buf) {

	char c = fgetc(in);

	int count = 0;
	while (c != '\n' && c != EOF && count < MAXLINE) {
			buf[count] = c;
			c = fgetc(in);
			count++;
	}
	if (c == '\n') {
			buf[count] = '\n';
	}
	if (c == EOF) {
			return -1;
	}
	return count;
}


void print_line(FILE *out, const char *buf) {
	fprintf(out, buf);
}

unsigned count_occurrences(const char *line, const char *str) {
	int matches;
	int occurrences = 0;
	int line_len = find_string_length(line);
	int search_word_len = find_string_length(str);

	for (int i = 0; i < line_len; i++) {
		matches = 0;
		for (int j = 0; j < search_word_len; j++) {
			if (line[i+j] == str[j]) {
				matches++;			
			}
		}
		if (matches == search_word_len) {
			occurrences++;
		}
	}
	return occurrences;
}
unsigned find_string_length(const char *s) {
	int tot_len = 0;
	char c = *s;

	while(c != '\n' && c != '\0') {
		c = *(s+tot_len);
		tot_len++;
	}
	return tot_len - 1;
}

int starts_with(const char *s, const char *pfx) {
	
}
int strings_equal(const char *s1, const char *s2);