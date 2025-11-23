
Regex Lite Project Guidelines
=============================

# Primary Goals
1. Implement a recursive pattern-mathcing engine
2. Support simplified regex features for line-based text search
3. Demonstrate modular desing using header and source files
4. Enable file-based search with line-by-line matching
5. Provide a foundation for extending to full regex capabilities

## "Regex" Definition
- Regex: "regular expression"
    - A sequence of characters that defines a search pattern
    - Used for string matching and manipulation
    - Commonly used in text processing, data validation, and search algorithms

## "Recursion" Definition
- Recursion: a function calls itself to solve smaller instances
    of the same problem until a "base case" is met, which stops the process.
- Recursive call: the function calls itslef with a modified input
    that moves it closer to the base case.
- Base case: A condition that tells the function when to stop recuring.
- Stack frame: Each time a function is called, a new stack frame is created
    to hold the function's parameters, local variables, and return address.
    - When the function returns, its stack frame is removed.
- **WITHOUT A BASE CASE, THE FUNCTION WILL RECURSE INDEFINITELY,
    LEADING TO A STACK OVERFLOW ERROR.**

### Engine must recognize specific symbols to perform searches.
- **'^'** :     Start of line (Anchor)
- **'$'** :     End of line (Anchor)
- **'[abc]'** : Character class (matches any one of the enclosed characters)
- **'*'** :     Zero or more repetitions of the preceding element
- **'?'** :     Zero or one repetition of the preceding element
- **'.'** :     Wildcard (matches any single character)
- **'+'** :     One or more repetitions of the preceding element
- **'{n}'** :   Exactly n repetitions of the preceding element
- **'{n, m}'** : Between n and m repetitions of the preceding element

### Function Syntax
```C
+ int match(const char *pattern, const char *text);
+     // Returns 1 if the pattern matches the text, 0 otherwise
+ int match_here(const char *pattern, const char *text);
+     // Helper function to match the pattern at the current position in the text
+ int match_star(char c, const char *pattern, const char *text);
+     // Handles the '*' operator for zero or more repetitions of character c
+ int match_plus(char c, const char *pattern, const char *text);
+     // Handles the '+' operator for one or more repetitions of character c
+ int match_question(char c, const char *pattern, const char *text);
+     // Handles the '?' operator for zero or one repetition of character c
+ int match_braces(char c, int n, int m, const char *pattern, const char *text);
+     // Handles the '{n}' and '{n,m}' operators for specified repetitions of character c
+ int is_char_class_match(char c, const char *char_class);
+     // Checks if character c matches the character class defined in char_class
+ int match_bracket(const char *char_class, char c);
+     // Handles character classes defined within brackets []
+ int search_file(const char *filename, const char *pattern);
+     // Reads the input file line by line and applies match() (aka the regex engine) to each line
        // and prints the matching lines along with their line numbers.
```

### Example Patterns
```C
./regex-lite test_cases.txt "^The" 
    // Matches lines starting with "The"
./regex-lite test_cases.txt "end$"
    // Matches lines ending with "end"
./regex-lite test_cases.txt "a*b"
    // Matches lines with zero or more 'a's followed by 'b'
./regex-lite test_cases.txt "colou?r"
    // Matches "color" or "colour"
./regex-lite test_cases.txt "h.llo"
    // Matches "hello", "hallo", "hxllo", etc.
./regex-lite test_cases.txt "go+gle"
    // Matches "gogle", "google", "gooogle", etc.
./regex-lite test_cases.txt "ha{2}!"
    // Matches "haa!"
./regex-lite test_cases.txt "ha{2,4}!"
    // Matches "haa!", "haaa!", or "haaaa!"
```

## Output Format
The program outputs the line number followed by the text of each matching line.
*/