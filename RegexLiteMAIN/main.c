/*
MAIN PROGRAM FILE
This file handles the arguments and file reading for the regex engine.
*/

#include <stdio.h>
#include <stdlib.h>
#include "regex.h"
#include <time.h> // Included for timing benchmarks
#include <string.h> // Included for string manipulation functions to resolve newline issues in end anchor matching and negation

#ifdef _WIN32
#include <windows.h>
#endif

#define MAX_LINE_LENGTH 1024 // Set a reasonable buffer size for reading lines

/*  * interactive mode: Runs a loop allowing the user to type regex/text manually
    * Triggered when the user runs the program with NO arguments
*/
void intercative_mode() {
    char pattern[MAX_LINE_LENGTH];
    char text[MAX_LINE_LENGTH];

    printf("===========================================\n");
    printf("    Regex Lite Interactive Mode\n");
    printf("    type 'exit' as pattern to quit.\n");
    printf("===========================================\n\n");

    while (1) {
        // Get pattern from user
        printf("Enter regex pattern: ");
        if (fgets(pattern, sizeof(pattern), stdin) == NULL)
            break;
        // Remove trailing newline
        pattern[strcspn(pattern, "\n")] = '\0';
        // Check for exit command
        if (strcmp(pattern, "exit") == 0) {
            printf("Exiting...\n");
            break;
        }

        // Get text from user
        printf("Enter text to match: ");
        if (fgets(text, sizeof(text), stdin) == NULL)
            break;
        // Remove trailing newline
        text[strcspn(text, "\n")] = '\0';

        // Perform matching
        if (match(pattern, text)) {
            printf("Match!\n\n");
        } else {
            printf("No match.\n\n");
        }
    }
}

/*
Acts as the "manager" of the regex engine.
It doesn't know how to match patterns itself; it just feeds lines of text to the match()
function and acts on the result (the 1 or 0 return value).
*/
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
    // Scenario 1: No arguments - enter interactive mode
    if (argc == 1) {
        intercative_mode();
        return 0;
    }
    // scenario 2: Two arguments - file and pattern
    // Check for correct number of arguments
    if (argc != 3) {
        printf("Usage: %s <file> <pattern>\n", argv[0]);
        return 1;
    }

    const char *file = argv[1];
    const char *pattern = argv[2];

    // High-resolution timing (wall-clock).
    // Use QueryPerformanceCounter on Windows, fallback to clock() if unavailable.
    // This measures real elapsed time, not CPU time, and can fall back to clock if machine support is lacking.
#ifdef _WIN32
    LARGE_INTEGER freq, t1, t2;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&t1);
    // Search the file for the pattern
    search_file(file, pattern);
    QueryPerformanceCounter(&t2);
    double elapsed_ms = (double)(t2.QuadPart - t1.QuadPart) * 1000.0 / (double)freq.QuadPart;
    printf("\nSearch completed in %.3f milliseconds.\n", elapsed_ms);
#else
    // POSIX fallback: use clock_gettime when available for monotonic timing
#if defined(CLOCK_MONOTONIC)
    struct timespec ts1, ts2;
    clock_gettime(CLOCK_MONOTONIC, &ts1);
    search_file(file, pattern);
    clock_gettime(CLOCK_MONOTONIC, &ts2);
    double elapsed_ms = (ts2.tv_sec - ts1.tv_sec) * 1000.0 + (ts2.tv_nsec - ts1.tv_nsec) / 1e6;
    printf("\nSearch completed in %.3f milliseconds.\n", elapsed_ms);
#else
    // Fallback to clock() (CPU time) if no monotonic clock available
    clock_t start_time = clock();
    search_file(file, pattern);
    clock_t end_time = clock();
    double elapsed_ms = ((double)(end_time - start_time)) * 1000.0 / CLOCKS_PER_SEC;
    printf("\nSearch completed in %.3f milliseconds (CPU time).\n", elapsed_ms);
#endif
#endif

    return 0;
}