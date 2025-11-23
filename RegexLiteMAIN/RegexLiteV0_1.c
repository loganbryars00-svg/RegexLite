/*
IMPLEMENTATION OF REGEX ENGINE FUNCTIONS
This file contains the recursive logic for matching
regular expressions against text.
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "regex.h"

/* Function prototypes */
int match(const char *regexp, const char *text);
int match_here(const char *regexp, const char *text);
int match_star(int c, const char *regexp, const char *text);
int match_plus(int c, const char *regexp, const char *text);
int match_question(int c, const char *regexp, const char *text);
int match_braces(char c, int n, int m, const char *regexp, const char *text);
int match_bracket(const char *char_class, char c);

/* Main function to initiate the matching process
   Returns 1 if the pattern matches the text, 0 otherwise */
int match(const char *regexp, const char *text) {
    if (regexp[0] == '^')
        return match_here(regexp + 1, text);

    /* If no leading '^', try to match at any position */
    do {
        if (match_here(regexp, text))
            return 1;
    } while (*text++ != '\0');

    return 0;
}

/* Helper function to match the pattern at the current position in the text.
   This function consumes one construct at a time and recurses. */
int match_here(const char *regexp, const char *text) {
    if (regexp[0] == '\0')
        return 1;

    if (regexp[0] == '$' && regexp[1] == '\0')
        return *text == '\0';

    /* Handle constructs where the next char is a repetition operator */
    if (regexp[1] == '*')
        return match_star((unsigned char)regexp[0], regexp + 2, text);
    if (regexp[1] == '+')
        return match_plus((unsigned char)regexp[0], regexp + 2, text);
    if (regexp[1] == '?')
        return match_question((unsigned char)regexp[0], regexp + 2, text);

    /* Handle {n,m} after a char */
    if (regexp[1] == '{') {
        /* parse numbers n and m from {n,m} */
        const char *p = regexp + 2; /* after '{' */
        int n = 0, m = -1;
        /* parse n */
        while (isdigit((unsigned char)*p)) {
            n = n * 10 + (*p - '0');
            p++;
        }
        if (*p == ',') {
            p++;
            if (isdigit((unsigned char)*p)) {
                m = 0;
                while (isdigit((unsigned char)*p)) {
                    m = m * 10 + (*p - '0');
                    p++;
                }
            } else {
                m = -1; /* open upper bound */
            }
        } else {
            /* exact {n} form */
            m = n;
        }
        if (*p == '}') {
            /* proceed with braces matching: regexp points to char, full braces start at regexp+1 */
            return match_braces(regexp[0], n, m, p + 1, text);
        }
        /* if parsing failed, fall through to normal char matching */
    }

    /* Handle bracket character class */
    if (regexp[0] == '[') {
        /* find end of bracket */
        const char *end = regexp;
        while (*end != '\0' && *end != ']') end++;
        if (*end == ']') {
            if (*text != '\0' && match_bracket(regexp, *text))
                return match_here(end + 1, text + 1);
            else
                return 0;
        }
        /* malformed class, treat '[' as literal */
    }

    /* Handle '.' wildcard or literal character */
    if (*text != '\0' && (regexp[0] == '.' || regexp[0] == *text))
        return match_here(regexp + 1, text + 1);

    return 0;
}

/* Greedy '*' matching: try the longest possible stretch, then backtrack */
int match_star(int c, const char *regexp, const char *text) {
    const char *t = text;
    /* advance t to the end of the maximal run that matches c (or '.') */
    while (*t != '\0' && (c == '.' || *t == (char)c))
        t++;
    /* try to match the rest starting from the longest run, backtracking downwards */
    for (; t >= text; t--) {
        if (match_here(regexp, t))
            return 1;
        if (t == text) break; /* avoid underflow */
    }
    return 0;
}

/* '+' requires at least one match, then behaves like '*' */
int match_plus(int c, const char *regexp, const char *text) {
    const char *t = text;
    /* first char must match */
    if (*t == '\0' || (c != '.' && *t != (char)c))
        return 0;
    t++; /* consume one */
    /* now reuse star logic for the rest */
    while (*t != '\0' && (c == '.' || *t == (char)c))
        t++;
    for (; t >= text + 1; t--) {
        if (match_here(regexp, t))
            return 1;
        if (t == text + 1) break;
    }
    return 0;
}

/* '?' allows zero or one occurrence */
int match_question(int c, const char *regexp, const char *text) {
    /* try zero occurrences */
    if (match_here(regexp, text))
        return 1;
    /* try one occurrence */
    if (*text != '\0' && (c == '.' || *text == (char)c))
        return match_here(regexp, text + 1);
    return 0;
}

/* {n,m} matching: match at least n, at most m (m==-1 means no upper bound) */
int match_braces(char c, int n, int m, const char *regexp, const char *text) {
    const char *t = text;
    int count = 0;
    /* match at least n */
    while (count < n) {
        if (*t == '\0' || (c != '.' && *t != c))
            return 0;
        t++;
        count++;
    }
    /* Now count is n, find maximum reachable (up to m if specified) */
    const char *max_t = t;
    if (m == -1) {
        while (*max_t != '\0' && (c == '.' || *max_t == c))
            max_t++;
    } else {
        while (count < m && *max_t != '\0' && (c == '.' || *max_t == c)) {
            max_t++;
            count++;
        }
    }
    /* try to match remaining regexp with the largest run first (greedy), backtrack */
    for (const char *p = max_t; p >= text + n; p--) {
        if (match_here(regexp, p))
            return 1;
        if (p == text + n) break;
    }
    return 0;
}

/* Match a bracket expression like [abc] or [a-z] or [^0-9] */
int match_bracket(const char *char_class, char c) {
    const char *p = char_class + 1; /* skip '[' */
    int negate = 0;
    if (*p == '^') {
        negate = 1;
        p++;
    }
    int match_found = 0;
    while (*p != ']' && *p != '\0') {
        if (*(p + 1) == '-' && *(p + 2) != ']' && *(p + 2) != '\0') {
            /* range */
            char start = *p;
            char end = *(p + 2);
            if (start <= end) {
                if (c >= start && c <= end)
                    match_found = 1;
            } else {
                /* reversed range - treat conservatively */
                if (c >= end && c <= start)
                    match_found = 1;
            }
            p += 3;
        } else {
            if (*p == c)
                match_found = 1;
            p++;
        }
    }
    return negate ? !match_found : match_found;
}