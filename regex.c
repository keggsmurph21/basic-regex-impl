/**
 * Regular expression matcher, originally written by Rob Pike, related by Brian
 * Kernighan via https://www.cs.princeton.edu/courses/archive/spr09/cos333/beautiful.html.
 *
 * features:
 *  - c     matches any literal character c
 *  - .     matches any single character
 *  - ^     matches the beginning of the input string
 *  - $     matches the end of the input string
 *  - *     matches zero or more occurrences of the previous character (longest match)
 *  - *?    matches zero or more occurrences of the previous character (shortest match)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "regex.h"

int match(char *regexp, char *text)
{
    if (regexp[0] == '^')
        return match_here(regexp+1, text);
    do {
        if (match_here(regexp, text))
            return 1;
    } while (*text++ != '\0');
    return 0;
}

int match_here(char *regexp, char *text)
{
    if (regexp[0] == '\0')
        return 1;
    if (regexp[0] == '$' && regexp[1] == '\0')
        return *text == '\0';
    if (regexp[1] == '*') {
        if (regexp[2] == '?')
            return match_star(regexp[0], regexp+2, text);
        else
            return match_star_greedy(regexp[0], regexp+3, text);
    }
    if (*text != '\0' && (
                regexp[0] == '.' || regexp[0] == *text))
        return match_here(regexp+1, text+1);
    return 0;
}

int match_star(char c, char *regexp, char *text)
{
    do {
        if (match_here(regexp, text))
            return 1;
    } while (*text != '\0' && (*text++ == c || c == '.'));
    return 0;
}

int match_star_greedy(char c, char *regexp, char *text)
{
    char *t;

    for (t = text; *t != '\0' && (*t == c || c == '.'); t++)
        ;
    do {
        if (match_here(regexp, t))
            return 1;
    } while (t-- > text);
    return 0;
}

void do_test(char *regexp, char *text, int expect)
{
    if (match(regexp, text) != expect) {
        printf("test failed for match /%s/%s/\n", regexp, text);
        exit(1);
    } else {
        printf("test succeeded for match /%s/%s/\n", regexp, text);
    }
}

void do_tests(char *regexp, int e1, int e2)
{
    char text[100];

    strcpy(text, "");
    do_test(regexp, text, e1);

    strcpy(text, "sprint");
    do_test(regexp, text, e2);
}

int main(int argc, char **argv)
{
    char regexp[100];

    strcpy(regexp, "");
    do_tests(regexp, 1, 1);

    strcpy(regexp, "print");
    do_tests(regexp, 0, 1);

    strcpy(regexp, "^print");
    do_tests(regexp, 0, 0);

    strcpy(regexp, "print$");
    do_tests(regexp, 0, 1);

    strcpy(regexp, ".print");
    do_tests(regexp, 0, 1);

    strcpy(regexp, "^.print");
    do_tests(regexp, 0, 1);

    strcpy(regexp, ".*int");
    do_tests(regexp, 0, 1);
}
