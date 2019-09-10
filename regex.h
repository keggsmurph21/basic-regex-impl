#ifndef REGEX_H
#define REGEX_H

int match(char *regexp, char *text);
int match_here(char *regexp, char *text);
int match_star(char c, char *regexp, char *text);

#endif
