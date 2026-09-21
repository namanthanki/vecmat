CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LIBS = -lm
CPPFLAGS = -Iinclude

ifeq ($(OS),Windows_NT)
TARGET = demo_vector.exe
RM = del /Q
else
TARGET = demo_vector
RM = rm -f
endif

SOURCES = examples/demo_vector.c src/vector.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	-$(RM) $(TARGET)