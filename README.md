# Regex Lite

**Regex Lite** is a lightweight, recursive text search engine implemented in C. Designed for educational purposes, it mimics the core functionality of standard regular expression engines (like grep) to demonstrate string parsing, recursion, and modular software design.

This project was developed for ECE3331 (Fall 2025).

## Features

Regex Lite supports a robust subset of standard regex syntax, including greedy quantifiers, character classes, and anchors.

### Core Capabilities

- **Recursive Descent Architecture:** Uses a pure recursive strategy to handle complex pattern matching and backtracking.

- **Modular Design:** Clean separation between the User Interface (main.c) and the Regex Engine (RegexLiteV1.c).

- **Dual Operation Modes:**

  - **File Search Mode:** Scans a file line-by-line for matches.

  - **Interactive Mode:** A console-based shell for testing patterns instantly.

- **Performance Benchmarking:** Built-in high-resolution timing to measure search execution speed.

- **Case Insensitivity:** All matching is case-insensitive by default (e.g., a matches A).

## Supported Syntax

The engine recognizes the following special symbols:

| Symbol | Name | Description | Example | Matches |
| :--- | :--- | :--- | :--- | :--- |
| ^ | Start anchor | Matches the beginning of the line. | ^The | "The end" |
| $ |End anchor | Matches the end of the line. | end$ | "The end" |
| . | Wildcard | Matches any single character. | h.t | "hot", "hat" |
| * | Star | Zero or more repetitions (Greedy). | a*b | "b", "ab", "aaab" |
| + | Plus | One or more repetitions (Greedy). | a+b | "ab", "aaab" |
| ? | Question | Zero or one repetition (Greedy). | colou?r | "color", "colour" |
| {n} | Exact Count | Exactly n repetitions. | go{2}gle | "google" |
| {n,m} | Range Count | Between n and m repetitions. | ha{2,4} | "haa", "haaaa" |
| [...] | Char Class | Matches any character inside brackets. | [Hh]ello | "Hello", "hello" |
| [^...] | Negation | Matches any character NOT inside brackets. | [^0-9] | Any non-digit |
| [a-z] | Ranges | Supports ASCII ranges inside brackets. | [a-z] | Any lowercase letter |

## Installation & Compilation

**Prerequisites**

- GCC Compiler (MinGW for Windows, or standard GCC for Linux/macOS)

- Make tool

**Build Instructions**

The project includes a **Makefile** for automated compilation.

1. Open your terminal in the project directory.

2. Run the make command:
```CLR
make
```

3. This will generate an executable named regex-lite (or regex-lite.exe on Windows).

To clean up build files:
```CLR
make clean
```

## Usage

### 1. Interactive Mode (Console)

Run the program without arguments to enter the interactive shell. This is perfect for testing patterns quickly.
```CLR
./regex-lite
```

**Example Session:**
```CLR
===========================================
    Regex Lite Interactive Mode
    type 'exit' as pattern to quit.
===========================================

Enter regex pattern: h.llo
Enter text to match: Hello World
Match!
```

### 2. File Search Mode

Run the program with two arguments: the input filename and the regex pattern.
```CLR
./regex-lite <filename> <pattern>
```

**Example:**
Find all lines starting with "The" in test_cases.txt:
```CLR
./regex-lite test_cases.txt "^The"
```

**Output:**
```CLR
5: The quick brown fox
6: The end

Search completed in 0.042 milliseconds.
```

## Project Structure

**- main.c:** The "Manager." Handles command-line arguments, file I/O, the interactive loop, and performance timing. It calls the engine but does not know how matching works.

**- RegexLiteV1.c:** The "Engine." Contains the implementation of the recursive matching logic (match, match_here, match_star, etc.).

**- regex.h:** The "Interface." A header file defining the function prototypes exposed by the engine.

**- Makefile:** Automation script for compiling and linking the project.

**- test_cases.txt:** A standard text file containing various strings for functional testing.

**- stresstest.txt:** A large file used to benchmark the engine's speed and stability.

## Technical Details

### Architecture

The engine relies on Recursive Descent Parsing.

**1. match():** The entry point. It handles the ^ anchor. If absent, it slides along the text calling match_here at every position.

**2. match_here():** The dispatcher. It looks at the current character in the pattern and decides which helper function to call (e.g., if it sees *, it calls match_star).

**3. Backtracking:** Functions like match_star and match_braces are "Greedy." They consume as much text as possible first. If the rest of the pattern fails to match, they "backtrack" (give back characters one by one) and try again.

### Memory Safety

The system uses fixed-size buffers (MAX_LINE_LENGTH = 1024) in main.c to prevent unbounded input reading.

The engine operates purely on pointers (char *), avoiding unnecessary memory allocation (malloc) during the matching process.

## Testing

We have verified the engine using test_cases.txt against the following scenarios:

- Anchors: ^The, end$

- Quantifiers: go*gle, go+gle, colou?r

- Complex Classes: [a-z], [^0-9]

- Edge Cases: Empty strings, patterns longer than text, and rapid repetition.

Performance is measured using high-resolution wall-clock timing (Windows QueryPerformanceCounter or POSIX clock_gettime).

## Credits

**Team Members:**

Logan Bryars, Danase Dubois, Daniella Nguenti Tchoualong, Phillip Simpson, Shanay-Abbas Soonasra

University of Houston

Course: ECE3231 - Fall 2025
