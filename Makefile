CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -pthread
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = http_server

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) -pthread

clean:
	rm -f src/*.o $(TARGET)

run: all
	./$(TARGET)