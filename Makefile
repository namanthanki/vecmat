CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LIBS = -lm
CPPFLAGS = -Iinclude -Iinclude/vecmat

ifeq ($(OS),Windows_NT)
TARGET = demo_vector.exe
NORMS_TARGET = demo_norms_independence.exe
RM = del /Q
else
TARGET = demo_vector
NORMS_TARGET = demo_norms_independence
RM = rm -f
endif

SOURCES = src/vector.c

all: $(TARGET) $(NORMS_TARGET)

$(TARGET): examples/demo_vector.c $(SOURCES)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)

$(NORMS_TARGET): examples/demo_norms_independence.c $(SOURCES)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	-$(RM) $(TARGET) $(NORMS_TARGET)