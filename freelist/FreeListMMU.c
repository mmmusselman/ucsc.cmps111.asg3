/*Matthew Musselman (W1202889) for P3 in CS111 (Miller, Spring 2012)*/

/**********************************************************************************
*  FreeListMMU.c
*  Implementation file for FreeList MMU
***********************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "FreeList.h"
#include "FreeListMMU.h"

/*
*  freelistinit
*/
void *freelistinit(long n_bytes, unsigned int flags);

/*
*  freelistalloc
*/
void *freelistalloc(char *mem, long n_bytes);

/*
*	freelistfree
*/
void freelistfree(char *mem, int address);

