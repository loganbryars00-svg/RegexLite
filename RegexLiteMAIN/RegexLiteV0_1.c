
#include <stdio.h>
#include <stdlib.h>
// Function prototypes
/*

*/

// Helper function to match the pattern at the current position in the text
int match_here(const char *regexp, const char *text){
    // Implementation of match_here function
    if (regexp[0] == '\0')
        return 1; // Base case: empty pattern matches any text
    if (regexp[1] == '*')
        return match_star(regexp[0], regexp + 2, text);
    if (*text != '\0' && (regexp[0] == '.' || regexp[0] == *text))
        return match_here(regexp + 1, text + 1);
    
    return 0; // No match
}

/*
 * Implements greedy matching for the '*' operator.
 * Attempts to match as many occurrences of character 'c' (or '.' wildcard) as possible,
 * then recursively checks if the remaining pattern matches the rest of the text.
 * This strategy ensures the longest possible match before backtracking.
 */
int match_star(int c, const char *regexp, const char *text){
    const char *t;
    // Loop through text while current character matches 'c' or if 'c' is a wildcard '.'
    for (t = text; *t != '\0' && (*t == c || c == '.'); t++) {
        if (match_here(regexp, t))
            return 1; // Match found
    }

    return 0; // No match
}

// Main function to initiate the matching process
// Returns 1 if the pattern matches the text, 0 otherwise
int match(const char *regexp, const char *text){
    // Implementation of match function
    if (regexp[0] == '^')
        return match_here(regexp + 1, text);
    
    do {
        if (match_here(regexp, text))
            return 1;
    } while (*text++ != '\0');
    
    return 0; // No match
}

// Handles the '+' operator for one or more repetitions of character c
int match_plus(int c, const char *regexp, const char *text){
    const char *t;
    // Ensure at least one occurrence of 'c' (or '.' wildcard)
    if (*text == '\0' || (*text != c && c != '.'))
        return 0; // No match

    // Match one or more occurrences of 'c'
    for (t = text + 1; *t != '\0' && (*t == c || c == '.'); t++) {
        if (match_here(regexp, t))
            return 1; // Match found
    }

    return 0; // No match
}

int match_question(int c, const char *regexp, const char *text){
    // Try matching without the character
    if (match_here(regexp, text))
        return 1;

    // Try matching with one occurrence of the character
    if (*text != '\0' && (*text == c || c == '.'))
        return match_here(regexp + 1, text + 1);

    return 0; // No match
}

int match_braces(char c, int n, int m, const char *regexp, const char *text){
    const char *t = text;
    int count = 0;

    // Match at least 'n' occurrences of 'c'
    while (count < n) {
        if (*t == '\0' || (*t != c && c != '.'))
            return 0; // No match
        t++;
        count++;
    }

    // Try matching with additional occurrences up to 'm'
    while (count <= m && (*t == c || c == '.')) {
        if (match_here(regexp, t))
            return 1; // Match found
        t++;
        count++;
    }

    return 0; // No match
}

int is_char_class_match(char c, const char *char_class){
    // Implementation of character class matching
    char_class++; // Skip the opening '['
    while (*char_class != ']' && *char_class != '\0') {
        if (*char_class == c)
            return 1; // Match found
        char_class++;
    }
    return 0; // Placeholder
}
int match_bracket(const char *char_class, char c){
    // Implementation of bracket character class matching
    
    return 0; // Placeholder
}

/*
Acts as the "manager" of the regex engine.
It doesn't know how to match patterns itself; it just feeds lines of text to the match()
function and acts on the result (the 1 or 0 return value).
*/
int search_file(const char *filename, const char *pattern){
    // Implementation of file search function
    // Open file for reading
    FILE *file = fopen(filename, "r");
    // Check if the file exists
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line_buffer[MAX_LINE_LENGTH]; // buffer to store the current line
    int line_number = 0;

    // Read the file line by line
    // fegets returns NULL when end of file is reached
    while (fgets(line_buffer, sizeof(line_buffer), file) != NULL) {
        line_number++; // Increment line number
        // CRITICAL: Check if the current line matches the pattern
        // Call match(). If it returns 1, print the line number and line text
        if (match(pattern, line_buffer)) {
            printf("%d: %s", line_number, line_buffer);
        }
    }
    // Clean up: close the file
    fclose(file);
}