CC=g++
CFLAGS=-I.
DEPS = chat.h
OBJ = chat.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)
	$(CC) -o chat $^ $(CFLAGS)