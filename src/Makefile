TARGET = prompter
LIBS = -lcurl -lxml2 -lm -lncurses
CC = gcc
CFLAGS = -std=c11 -g -Wall -I/usr/include/libxml2
RM = rm -f

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))

%.o : %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall -g $(LIBS) -o $@

clean:
	$(RM) $(OBJECTS)
	$(RM) $(TARGET)
