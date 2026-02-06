CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11
LDFLAGS = -lcurl

SRCS = main.c cJSON.c
TARGET = weather

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET) Moscow
