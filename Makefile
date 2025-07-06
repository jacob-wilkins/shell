CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = shell

SRC = src/main.c lib/utils.c lib/commands.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# This pattern matches objects in any subdirectory
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

run:
	gnome-terminal -- ./$(TARGET)

.PHONY: all clean run