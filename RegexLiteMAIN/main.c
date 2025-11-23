/*
MAIN PROGRAM FILE
This file handles the arguments and file reading for the regex engine.
*/

#include <stdio.h>
#include <stdlib.h>
#include "regex.h"

/*
Acts as the "manager" of the regex engine.
It doesn't know how to match patterns itself; it just feeds lines of text to the match()
function and acts on the result (the 1 or 0 return value).
*/
#define MAX_LINE_LENGTH 1024 // Set a reasonable buffer size for reading lines

int search_file(const char *filename, const char *pattern){
    // Implementation of file search function
    // Open file for reading
    FILE *file = fopen(filename, "r");
    // Check if the file exists
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    char line_buffer[MAX_LINE_LENGTH]; // buffer to store the current line
    int line_number = 0;

    // Read the file line by line
    // fgets returns NULL when end of file is reached
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
    return 0;
}

int main(int argc, char *argv[]){
    // Check for correct number of arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <pattern> <filename>\n", argv[0]);
        return 1;
    }

    const char *pattern = argv[1];
    const char *filename = argv[2];

    // Search the file for the pattern
    search_file(filename, pattern);

    return 0;
}