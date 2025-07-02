# Guidance from makefile tutorial on Canvas
# Adapted from Bryan's makefile for Assignment 1

CC=gcc
CFLAGS=-O3 -Wall
EXE=htproxy
OBJ_EXE=stage1.o stage2.o stage3.o stage4.o 

$(EXE): main.c $(OBJ_EXE)
	$(CC) $(CFLAGS) -g -o $@ $(OBJ_EXE) $<

%.o: %.c %.h
	$(CC) $(CFLAGS) -g -c -o $@ $<

clean:
	rm -f *.o $(EXE)

format:
	clang-format -style=file -i *.c *.h

# valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./htproxy -p 8080 -c