# Makefile for Programming Assignment 5
# Matthew Musselman (W1202889)

all : FreeListMMUTest FreeListTest

FreeListMMUTest : FreeListMMUTest.o FreeListMMU.o FreeList.o
	gcc -w -o FreeListMMUTest FreeListMMUTest.o FreeListMMU.o FreeList.o

FreeListMMUTest.o : FreeListMMUTest.c FreeList.h
	gcc -w -c -ansi -Wall FreeListMMUTest.c

FreeListMMU.o : FreeListMMU.c FreeList.h FreeListMMU.h
	gcc -w -c -ansi -Wall FreeListMMU.c

FreeListTest : FreeListTest.o FreeList.o
	gcc -w -o FreeListTest FreeListTest.o FreeList.o

FreeListTest.o : FreeListTest.c FreeList.h
	gcc -w -c -ansi -Wall FreeListTest.c

FreeList.o : FreeList.c FreeList.h
	gcc -w -c -ansi -Wall FreeList.c

clean :
	rm -f FreeListTest FreeListMMUTest *.o
