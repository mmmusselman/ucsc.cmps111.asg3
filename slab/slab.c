#include <stdio.h>
#include <stdlib.h>
#include <lib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

//Look through slab + free list
//Add malloc to each case + calculation of how much memory needed



int meminit(long n_bytes, unsigned int flags, int parm1, int *parm2);
void *memalloc (int handle, long n_bytes);
void memfree (void *region);

char *alloc_array[512];
int curr_id = 0;


int main(){
   printf("Test application\n");
   printf("sizeof int %d\n", sizeof(char *));
   printf("int shift %d\n", (1<<1));
   int array[5] = {2, 4, 8, 16, 0};
   int handler1=meminit(4096,2,1,array);
   memalloc(handler1, 15);
   memalloc(handler1, 6);
   
   return 0;
}

int meminit(long n_bytes, unsigned int flags, int parm1, int *parm2){
    printf("n_bytes = %d\n", (int)n_bytes);
    printf("flags = %d\n", flags);
    printf("parm1 = %d\n", parm1);

	int pageSizeBytes = 4096;
    int numSlabs, allocSize, numSizes, headerSize, slabSize;
	int *ptr;
	int i, j;
	
    // 1. Save the flag number in the piece of memory itself or return an error
    // 2. Prepare allocators and metadata
    switch (flags)
    {
        // Buddy allocator
        case 0x1: 
            // If requested size is not a power of two, return an error
            if ((n_bytes & (n_bytes - 1)) != 0)
            {
                fprintf(stderr, "requested size for buddy allocator is not a power of 2\n");
                return -1;
            }
            break;
        // Slab allocator
        case 0x2:
			/*error detection stuff*/
			if(n_bytes < pageSizeBytes) {
				fprintf(stderr, "Cannot initialize less than a page with slab.\n");
				return -1;
			}
			numSlabs = (n_bytes/(parm1*pageSizeBytes)); //number of slabs in the allocator
			slabSize = n_bytes/numSlabs; //number of bytes in a slab
			ptr = parm2;
			
			for(numSizes = 0, i=0; ptr[i] != 0; i++)
				numSizes++;
			
			allocSize = sizeof(int) +  //flag
			    sizeof(int) +                        //numSlabs
				sizeof(int) +						 //slabSize
				sizeof(int) + 						 //numSizes
			   (sizeof(int) * numSizes) + //array for legal object sizes
			   (sizeof(int *) * numSlabs) + //current object size of each slab
			   (sizeof(char *) * numSlabs) + //array of pointers to first object of each slab
			   (sizeof(char *) * numSlabs) + //array of pointers to last object of each slab
			   (sizeof(char *) * numSlabs) + //array of pointers to first free object of each slab
			   (sizeof(char) * n_bytes);
			   
			   //sizeof(int *) == sizeof(char *) so this is less interesting than it appears
			   
			headerSize = allocSize - n_bytes;
			   
			printf("allocSize=%d\n", allocSize);
			printf("numSizes=%d\n", numSizes);
			printf("numSlabs=%d\n", numSlabs);
			if( NULL == (alloc_array[curr_id] = malloc(allocSize)) ) {           //allocate & check for error
				fprintf(stderr, "malloc failed in slab\n");
				return -1;
			}
			printf("lastAddress=%d\n", &alloc_array[curr_id][allocSize-1]);
			
			memset(alloc_array[curr_id], 0, allocSize); //initialize everything to 0
			
			((int *) alloc_array[curr_id])[0] = flags; //initialize flags
			((int *) alloc_array[curr_id])[1] = numSlabs; //initialize numSlabs
			((int *) alloc_array[curr_id])[2] = slabSize; //initialize slabSize
			((int *) alloc_array[curr_id])[3] = numSizes; //initialize numSizes
			ptr = parm2;
			for(i=0, j=4; i<numSizes; i++, j++) {
				((int *) alloc_array[curr_id])[j] = ptr[i]; //initialize valid object sizes
			}
			//j = current index in alloc_array
			for(i=0; i<numSlabs; i++, j++) {
				((int *) alloc_array[curr_id])[j] = 0; //initialize current slab object sizes to 0 which they already were.  redundancy...comment out later?
			}
			//initialize first object pointers
			for(i=0; i<numSlabs; i++, j++) {
				((int *) alloc_array[curr_id])[j] = (int)(&alloc_array[curr_id][headerSize + i*slabSize]);
			}
			//initialize last byte pointers
			for(i=1; i<=numSlabs; i++, j++) {
				((int *) alloc_array[curr_id])[j] = (int)(&alloc_array[curr_id][headerSize + i*slabSize - 1]);
			}
			//initialize first free object pointers to first object pointers for now
			for(i=0; i<numSlabs; i++, j++) {
				((int *) alloc_array[curr_id])[j] = (int)(&alloc_array[curr_id][headerSize + i*slabSize]);
			}
			
			
			
			//print the memory
			for(i=0; i<1024; i++) {
				printf("%d ", ((int*)alloc_array[curr_id])[i]);
			}
			puts("");
			
			
			break;
	
	
        // Free-list allocator
        case 0x4: case 0x4 | 0x08:  case 0x4 | 0x10: case 0x4 | 0x18:
             break;
        default: fprintf(stderr, "invalid flag\n"); return -1;
    }    
    
    // Before returning, prepare the global allocator array index for the next
    // time a new allocator is requested
    ++curr_id;
    
    // Id for each allocator is 100 + actual index in alloc_array
    return 99 + curr_id;
}

void *memalloc(int handle, long n_bytes){
   printf("h: %d\n", handle);
   int newH=handle-100;
   int flag = ((int *)alloc_array[newH])[0];
   
   int numSlabs, slabSize, numSizes, closestObjSize, fullSlabs, numObj;
   int *ptr, *ptr2, *ptr3, *ptr4;
   int *returnPtr, *x;
   int i, j;
   
   switch (flag){
      case 1:
            printf("Buddy\n");
            break;
      case 2: 
            printf("Slab\n");
			
			//get the number of slabs
			numSlabs = ((int *)alloc_array[newH])[1];
			slabSize = ((int *)alloc_array[newH])[2];
			numSizes = ((int *)alloc_array[newH])[3];

			ptr = &((int*)alloc_array[newH])[4];//valid object sizes
			for(i=0, closestObjSize=0; i<numSizes; i++) {
				if(n_bytes <= ptr[i]) {
					if(closestObjSize > ptr[i] || closestObjSize==0)
						closestObjSize=ptr[i];
				}
			}
			if(0 == closestObjSize)
				return; //n_bytes is bigger than all valid object sizes
			
			//Now have closestObjSize.

			printf("n_bytes=%d\n", n_bytes);
			printf("closestObjSize=%d\n", closestObjSize);
			
			ptr = &((int *)alloc_array[newH])[4+numSizes+numSlabs]; //first object pointer
			ptr2 = &((int *)alloc_array[newH])[4+numSizes]; //current slab object size
			ptr3 = &((int *)alloc_array[newH])[4+numSizes+3*numSlabs]; //first free object pointer
			ptr4 = &((int *)alloc_array[newH])[4+numSizes+2*numSlabs]; //last byte pointer

			//first pass: find perfect slab
			for(i=0; i<numSlabs; i++) {
				if(ptr2[i] == 0)
					continue;
				else if(ptr2[i] == closestObjSize) { //found perfect slab
					if(ptr3[i] > ptr4[i]) continue; //if full, not perfect after all
					
					returnPtr = ptr3[i];
					ptr3[i] = ptr3[i] + closestObjSize;//update ptr3[i]
					memset(returnPtr, 0, closestObjSize);
					//return returnPtr;
				}
			}
			//second pass: find empty slab
			for(i=0; i<numSlabs; i++) {
				if(ptr2[i] == 0) {//empty slab
					//split slab into closestObjSize (write nextFreeSlab ptrs)
					numObj = slabSize/closestObjSize;
					for(j=0; j<numObj-1; j++) { //last obj points to null
						x = ptr3[i]+j*closestObjSize;
						*x=ptr3[i]+(j+1)*closestObjSize; //Make first byte of each object point to first byte of next object
					}
					ptr2[i] = closestObjSize; //update current slab's object size
					returnPtr = ptr3[i]; //backup first free object
					ptr3[i] = ptr3[i]+closestObjSize;//update ptr3[i]
					memset(returnPtr, 0, closestObjSize);
					//return returnPtr;
				}
			}
			//third pass: find slab with bigger object size
			//j keeps track of the smallest bigger slab
			j=-1;
			for(i=0; i<numSlabs; i++) {
				if(n_bytes < ptr2[i] && ptr3[i]<ptr4[i]) {
					if(j==-1)
						j = i;
					else if(ptr2[i]<ptr2[j])
						j = i;
				}
				returnPtr = ptr3[j];
				ptr3[j] = ptr3[j] + closestObjSize;
				memset(returnPtr, 0, closestObjSize);
				//return returnPtr;
			}

			
			//pick the slab (get slab idx)
			//go through each first free object pointer and stop when you hit one NOT NULL of RIGHT OBJECT SIZE
				//if ALL are NULL/wrongobjectsize AND firstObjPtr==firstFreeObjPtr, then initialize the first free slab (IF n_bytes is valid object size)
					//initialize each object to point to the next (necessarily free) object.  last object points to null
					//update current slab object size
			
			//return first free obj ptr and set first free obj ptr to the next free obj ptr 
			for(i=0; i<1024; i++) {
				printf("%d ", ((int*)alloc_array[newH])[i]);
			}
			puts("");
            break;
      case 4: break;
   }
   
   return 0;
}