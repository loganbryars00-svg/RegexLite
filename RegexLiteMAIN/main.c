/*
MAIN PROGRAM FILE
This file handles the arguments and file reading for the regex engine.
*/

#include <stdio.h>
#include <stdlib.h>
#include "regex.h"
#include <time.h> // Included for timing benchmarks
#include <string.h> // Included for string manipulation functions to resolve newline issues in end anchor matching and negation

// Debugging includes - can be removed later
// #include <unistd.h>
// #include <limits.h>

/*
Acts as the "manager" of the regex engine.
It doesn't know how to match patterns itself; it just feeds lines of text to the match()
function and acts on the result (the 1 or 0 return value).
*/
#define MAX_LINE_LENGTH 1024 // Set a reasonable buffer size for reading lines

int search_file(const char *filename, const char *pattern) {
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
    int found = 0; // flag: set to 1 if we find at least one match

    // Read the file line by line
    // fgets returns NULL when end of file is reached
    while (fgets(line_buffer, sizeof(line_buffer), file) != NULL) {
        // Strip trailing newline(s): handle both '\n' and '\r' (Windows CRLF)
        size_t len = strlen(line_buffer);
        while (len > 0 && (line_buffer[len - 1] == '\n' || line_buffer[len - 1] == '\r')) {
            line_buffer[--len] = '\0';
        }
        // Now line_buffer ends exactly at the visible end of the line
        // Increment the line counter for each line read
        line_number++;
        if (match(pattern, line_buffer)) {
            printf("%d: %s\n", line_number, line_buffer);
            // note: I added '\n' explicitly since we stripped the newline
            found = 1;
        }
    }
    // Clean up: close the file
    fclose(file);
    // If no matches were found, inform the user
    if (!found) {
        printf("No matches found for pattern '%s' in file '%s'.\n", pattern, filename);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    // Check for correct number of arguments
    if (argc != 3) {
        printf("Usage: %s <file> <pattern>\n", argv[0]);
        return 1;
    }

    const char *file = argv[1];
    const char *pattern = argv[2];

    // Start timing (measured with clock())
    clock_t start_time = clock();
    // Search the file for the pattern
    search_file(file, pattern);
    clock_t end_time = clock();
    // End timing

    // Compute elapsed time in milliseconds
    double elapsed_ms = ((double)(end_time - start_time)) * 1000.0 / CLOCKS_PER_SEC;
    printf("\nSearch completed in %.3f milliseconds.\n", elapsed_ms);

    return 0;
}