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

# # --------- Compiler being used
all: rules.d $(EXE)

rules.d: Makefile $(wildcard *.c) $(wildcard *.h)
	gcc -MM $(wildcard *.c) >rules.d

# --------- rule for linking the executable product

-include rules.d

# --------- Define target "all" for building the executables
all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# --------- Rule for cleaning build and emacs artifacts
      clean:
				rm -f smash ./*.o ./*.stackdump EXE
