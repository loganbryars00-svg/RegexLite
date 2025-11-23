#ifndef REGEX_H
#define REGEX_H

int match(const char *regexp, const char *text);
int match_here(const char *regexp, const char *text);
int match_star(int c, const char *regexp, const char *text);
int match_plus(int c, const char *regexp, const char *text);
int match_question(int c, const char *regexp, const char *text);
int match_braces(char c, int n, int m, const char *regexp, const char *text);
int match_bracket(const char *char_class, char c);

#endif // REGEX_H