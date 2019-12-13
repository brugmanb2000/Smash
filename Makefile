 #------------------------------------------------------
#
# Makefile ---
#
# Usage
#      make all     Build the released product
#      make clean   Remove build artifacts
#
# Author: Brandon Brugman
# 10/31/2019
#
# ------------------------------------------------------

# # --------- Define names of all the object files in this project
        OBJS = smash.o history.o commands.o

# # --------- Define the name of the resulting released project
        EXE = smash

# --------- Define options passed by make to compiler
      CFLAGS = -std=c99 -Wall

%.o : %.c
	gcc $(CFLAGS) -c -o $@ $<


all: smash

smash: smash.o smashlib.a
	gcc -o $@ $^

debug: CFLAGS += -DDEBUG -g -Og
debug: smash

rules.d: Makefile $(wildcard *.c) $(wildcard *.h)
	gcc -MM $(wildcard *.c) >rules.d

-include rules.d

smashlib.a: history.o commands.o
					ar r $@ $^

valgrind: debug
	valgrind --leak-check=yes --leak-check=full --show-leak-kinds=all ./smash
# --------- Rule for cleaning build and emacs artifacts
      clean:
				rm -f smash ./*.o ./*.stackdump EXE ./*.md ./*.a ./*.out ./*.txt ./*.csv
