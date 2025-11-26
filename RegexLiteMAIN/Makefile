# Compiler to use

CC = gcc

# Compiler flags

# -Wall: Enable all warnings (helps find bugs)

# -Wextra: Enable even more warnings

# -std=c99: Use the C99 standard

CFLAGS = -Wall -Wextra -std=c99

# The name of the final executable program

TARGET = regex-lite

# The source files

SRCS = main.c RegexLiteV1.c

# The object files (based on source files)

OBJS = $(SRCS:.c=.o)

# Default target: just typing 'make' will run this

all: $(TARGET)

# Link the object files together to create the executable

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile each source file (.c) into an object file (.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files (typing 'make clean' runs this)

clean:
	rm -f $(OBJS) $(TARGET)