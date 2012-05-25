# Makefile for Programming Assignment 5
# Matthew Musselman (W1202889)

FreeListMMUTest : FreeList.o FreeListMMU.o
	gcc -o FreeListMMUTest FreeList.o FreeListMMU.o

FreeListMMU.o : FreeListMMU.c FreeListMMU.h FreeList.h
	gcc -c -ansi -Wall FreeListMMU.c
	
FreeList.o : FreeList.c FreeList.h
	gcc -c -ansi -Wall FreeList.c
	
clean :
	rm -f FreeListMMUTest *.o
