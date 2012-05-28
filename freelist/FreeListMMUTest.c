/*Matthew Musselman (W1202889) for PA5 in CS101 (Tantalo, Fall 2010)*/

/**********************************************************************************
*  ListTest.c
*  A test client for List ADT
***********************************************************************************/

#include<stdio.h>
#include"FreeList.h"

int main(int argc, char* argv[]) {
   int i;
   ListRef A = newList();
   ListRef B = newList();

   for(i=1; i<=10; i++)
   {
      insertAfterLast(A, i);
      insertAfterLast(B, 11-i);
   }
   printList(A);
   printf("\n");
   printList(B);
   printf("\n");

   for(i=1; i<=6; i++)
   {
      insertAfterLast(A, getFirst(B));
      printf("Inserted %d into List A\n", getFirst(B));
      deleteFirst(B);
      printf("Deleted that value from beginning of B, beginning is now %d\n", getFirst(B));
   }
   printList(A);
   printf("\n");
   printList(B);
   printf("\n");
   printf("%s\n", equals(A,B)?"true":"false");
   printf("%s\n", equals(A,A)?"true":"false");
   printf("\n");

   ListRef C = newList();
   for(i=1; i<=22; i++) {
	   insertAfterLast(C, i);
   }
   printList(C);
   printf("\n");
   insertInOrder(C, 11);
   printList(C);
   printf("\n");

   makeEmpty(C);

   freeList(&A);
   freeList(&B);
   freeList(&C);

   return(0);
}
