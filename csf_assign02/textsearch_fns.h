#ifndef TEXTSEARCH_FNS_H
#define TEXTSEARCH_FNS_H

#define MAXLINE 511

// TODO: declare helper functions here (to be implemented in
// c_textsearch_fns.c and asm_textsearch_fns.S)

int read_line(FILE *in, char *buf);
void print_line(FILE *out, const char *buf);
unsigned count_occurrences(const char *line, const char *str);
unsigned find_string_length(const char *s);
int starts_with(const char *s, const char *pfx);
int strings_equal(const char *s1, const char *s2);

#endif // TEXTSEARCH_FNS_H
