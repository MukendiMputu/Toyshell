# leads comments in a line
# Build all: default target
all : toyshell

# Separate compilation to build object files
tsh.o : tsh.c helpers.h
		gcc -c -ggdb tsh.c

helpers.o : helpers.c helpers.h
		gcc -c -ggdb helpers.c

# linking
#toyshell is a target which depends upon tsh.o and helpers.o
#"gcc tsh.o helpers.o -o toyshell" is the command to produce the executable file
#You need to use a TAB before gcc
toyshell : tsh.o helpers.o
		gcc tsh.o helpers.o -o toyshell

# Testing
check : all
		./toyshell

# Clean up all build targets so that one may get a clean build
clean :
		rm -f *.o toyshell core*
