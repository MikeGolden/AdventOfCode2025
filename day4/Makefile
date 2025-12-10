# C compiler
CC = gcc

# C flags
CFLAGS = -Wall -Wextra -pedantic -O2 -std=c11

# The target executable
TARGET = part1

# The source file
SRC = part1.c

# The default rule
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# To run the code
run: $(TARGET)
	./$(TARGET)

# To clean up the directory
clean:
	rm -f $(TARGET)
