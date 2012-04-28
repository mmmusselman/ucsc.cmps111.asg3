/*Matthew Musselman (W1202889) for P3 in CS111 (Miller, Spring 2012)*/

/**********************************************************************************
*  FreeListMMU.h
*  Header file for FreeList MMU
***********************************************************************************/
#if !defined(_FREELISTMMU_H_INCLUDE_)
#define _FREELISTMMU_H_INCLUDE_

/*
*  freelistinit
*/
void *freelistinit(long n_bytes, unsigned int flags);

/*
*  freelistalloc
*/
void *freelistalloc(char *mem, int flag, long n_bytes);

/*
*	freelistfree
*/
void freelistfree(char *mem, int address);

#endif
