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
   int i;
   int *alloc;
   //meminit(4096,2,1,array);
   //int handler1=meminit(4096,2,1,array);
   
   int handler2=meminit(4096*2,2, 1, array);
   
   
	for(i=0; i<2048+16; i++) {
		printf("%d ", ((int*)alloc_array[handler2-100])[i]);
	}
	puts("");
    //memalloc(handler2, 16);
   alloc=memalloc(handler2, 8);
   memalloc(handler2, 16);
   
   //alloc=memalloc(handler2, 16);
   *alloc=124;
   for (i=0; i<511; i++){
      printf("i:%d\n", i);
      alloc=memalloc(handler2, 8);
      *alloc = 124;
   }
   int *test;
   test=memalloc(handler2,8);
   printf("test: %d\n", test);
   if(!test){
      printf("EXIT!\n");
      return 0;
   }
   
  // memalloc(handler2, 8);
   for(i=0; i<2048+16; i++) {
		printf("%d ", ((int*)alloc_array[handler2-100])[i]);
	}
	puts("");
    printf("alloc is %d\n", alloc);
    for (i=0; i<512; i++){
       memfree(alloc-((i)*2));
    }
    /*
    //memalloc(handler2, 8);
    memalloc(handler2, 16);
    memalloc(handler2, 8);
    */
    for(i=0; i<2048+16; i++) {
		printf("%d ", ((int*)alloc_array[handler2-100])[i]);
	}
	puts("");

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
			/*for(i=0; i<1024; i++) {
				printf("%d ", ((int*)alloc_array[curr_id])[i]);
			}
			puts("");
			*/
			
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
            x=ptr3[0];
            printf("break\n");
            printf("ldf %d\n", x);
            
printf("**********1**************\n");
			//first pass: find perfect slab
			for(i=0; i<numSlabs; i++) {
                printf("slab#: %d ptr2[i]: %d\n", i, ptr2[i]);
				if(ptr2[i] == 0)
					continue;
                else if (ptr3[i]==0) //Slab is full
                    continue;
				else if(ptr2[i] == closestObjSize) { //found perfect slab
					if(ptr3[i] >= ptr4[i]) continue; //if full, not perfect after all
					printf("BE4ptr3[i]: %d\n", ptr3[i]);
                    x=ptr3[i];
                    printf("pttt: %d\n", *x);
                    //printf("pointing to... %d\n", *ptr3[i]);
					returnPtr = ptr3[i];
					if(ptr3[i]==NULL) {
                       printf("ptr3[i]null?: %d\n", ptr3[i]);
						x = ptr3[i];
                        printf("pointing to... %d\n", *x);
						*x = ptr4[i];
					} else {
                    
                       printf("ptr3[i]notnull?: %d\n", ptr3[i]);
						x = ptr3[i];
                        printf("pointing to... %d\n", *x);
						ptr3[i] = *x;
						//ptr3[i] = ptr3[i] + closestObjSize;//update ptr3[i]
					}
					memset(returnPtr, 0, closestObjSize);
                    printf("returnPrtr: %d\n", returnPtr);
					return returnPtr;
				}
			}
printf("**********2**************\n");
			//second pass: find empty slab
			for(i=0; i<numSlabs; i++) {
                if (ptr3[i]==0) continue; //slab is full
				else if(ptr2[i] == 0) {//empty slab
					//split slab into closestObjSize (write nextFreeSlab ptrs)
					numObj = slabSize/closestObjSize;
                    printf("%d dddptr3[i]: %d\n", i, ptr3[i]);
					for(j=0; j<numObj-1; j++) { //last obj points to null
                       //printf("closesobjsize: %d\n", closestObjSize);
						x = ptr3[i]+j*closestObjSize; //printf("%d ", x); 
						*x=ptr3[i]+(j+1)*closestObjSize; //Make first byte of each object point to first byte of next object
					     // printf("%d ", *x);
                    }
                    printf("last 0object at %d with %d value \n", x, *x);
                    x=x+4;
                    
                    printf("last 1object at %d with %d value \n", x, *x);
					ptr2[i] = closestObjSize; //update current slab's object size
					returnPtr = ptr3[i]; //backup first free object
					if(ptr3[i]==NULL) {
                       printf("null\n");
						x = ptr3[i];
						*x = ptr4[i];
					} else {
                        printf("else\n");
                        printf("ptr3in else: %d\n", ptr3[i]);
						x = ptr3[i];
                        printf("x is %d\n", *x);
						ptr3[i] = *x;
                       // x = ptr3[i];
                        //printf("making new : %d\n",*x); 
                       // if (*x==NULL)
						 //  ptr3[i] = *x;
						//ptr3[i] = ptr3[i] + closestObjSize;//update ptr3[i]
					}
                   // printf("last2 object at %d with %d value \n", x, *x);
					memset(returnPtr, 0, closestObjSize);
                    printf("\nkkptr3[i]: %d\n", ptr3[i]);
                    x = ptr3[i];
                    printf("return: %d\n", *x);
                    
                    for(i=0; i<1024; i++) {
		printf("%d ", ((int*)alloc_array[100-100])[i]);
	}
	puts("");
					return returnPtr;
				}
			}
printf("**********3**************\n");
			//third pass: find slab with bigger object size
			//j keeps track of the smallest bigger slab
			j=-1;
			for(i=0; i<numSlabs; i++) {
                if(ptr3[i]==0) continue;
				if(n_bytes < ptr2[i] && ptr3[i]<ptr4[i]) {
					if(j==-1)
						j = i;
					else if(ptr2[i]<ptr2[j])
						j = i;
				}
				returnPtr = ptr3[j];
				if(ptr3[i]==NULL) {
					x = ptr3[i];
					*x = ptr4[i];
				} else {
					x = ptr3[i];
					ptr3[i] = *x;
					//ptr3[i] = ptr3[i] + closestObjSize;//update ptr3[i]
				}
                if (j>-1){
				   memset(returnPtr, 0, ptr2[j]);
                   printf("ptr3[i]: %d\n", ptr3[i]);
				   return returnPtr;
                }
			}

			
			//pick the slab (get slab idx)
			//go through each first free object pointer and stop when you hit one NOT NULL of RIGHT OBJECT SIZE
				//if ALL are NULL/wrongobjectsize AND firstObjPtr==firstFreeObjPtr, then initialize the first free slab (IF n_bytes is valid object size)
					//initialize each object to point to the next (necessarily free) object.  last object points to null
					//update current slab object size
			
			//return first free obj ptr and set first free obj ptr to the next free obj ptr 
			
            break;
      case 4: break;
   }
   
   return 0;
}

void memfree(void *region) {
	int slabIdx, numSlabs, numSizes, slabSize, numObjects, firstFirstObjAddr, lastLastObjAddr;
	int i, counter, justLooping, lastFree;
	int *ptr, *ptr2, *ptr3, *ptr4, *test;
	int *nFOA; //next Free Object Address
	int *regionPtr;
	regionPtr = (long)region;
	
	int alloc_id = -1;
    long addr_diff = (1 << 30) - 1;
    //printf("addr_diff = %d\n", addr_diff);
    int argi;
    for (argi = 0; argi < 512; ++argi)
    {
        long diff = (long)(region) - (long)(alloc_array[argi]);
        if (diff < addr_diff && diff > 0)
        {
            alloc_id = argi;
            addr_diff = (long)(region) - (long)(alloc_array[argi]);
        }
    }
	printf("alloc_id = %d\n", alloc_id);
    printf("addr_diff = %lu\n", addr_diff);
	
	numSizes = ((int *)alloc_array[alloc_id])[3];
	numSlabs = ((int *)alloc_array[alloc_id])[1];
	ptr = &((int *)alloc_array[alloc_id])[4+numSizes+numSlabs]; //first object pointer
	ptr2 = &((int *)alloc_array[alloc_id])[4+numSizes]; //current slab object size
	ptr3 = &((int *)alloc_array[alloc_id])[4+numSizes+3*numSlabs]; //first free object pointer
	ptr4 = &((int *)alloc_array[alloc_id])[4+numSizes+2*numSlabs]; //last byte pointer
	
	//check if region is valid
	firstFirstObjAddr = ptr[0];
	lastLastObjAddr = ptr4[numSlabs-1];
	
	if(region < firstFirstObjAddr || region > lastLastObjAddr) return; //region invalid
	slabIdx=-1;
	for (i=0; i<numSlabs; i++){
      printf("i: %d\n", i);
	  if (region <= ptr4[i] && slabIdx==-1)   //Finds where the address is located in what slab #
		 slabIdx = i;
	  //else break;
	}
	printf("ptr4[0]:%d\n", ptr4[0]);
	slabSize = ((int *)alloc_array[alloc_id])[2];
    printf("slabsize: %d  slabID: %d\n", slabSize, slabIdx);
	numObjects = slabSize / ptr2[slabIdx];
	
	memset(region, 0, ptr2[slabIdx]);  //Clear object quickly
	test = region; 
	printf("before loop, ptr3[%d]=%d region %d regVal %d\n", slabIdx, ptr3[slabIdx], region, *regionPtr);
	nFOA = ptr3[slabIdx];
	
	justLooping = 0;
	counter = 0;
    lastFree = 0;
	printf("nFOA set %d\n",nFOA );
	while(nFOA < ptr4[slabIdx]) {	
		if(justLooping == 0) {
			if(ptr3[slabIdx] > region) { //region is the first free object
               printf("first Free\n");
				*regionPtr = ptr3[slabIdx];
                printf("nfoais %d", *nFOA);
                printf("x\n");
				ptr3[slabIdx] = region;
				justLooping = 1;
				counter++;
				counter++;//this works, trust me
			} else if(nFOA == 0) {
                printf("Last free\n");
				//nFOA is the address of the last free object
				if(region > nFOA) { //region is the new last free object
					//set *nFOA to region
					ptr3[slabIdx] = region;
                    memset(ptr3[slabIdx], 0, ptr2[slabIdx]); //all zero'd out
					//*region = NULL;
					justLooping = 1;
                    lastFree = 1;
					counter++;
                    printf("last count: %d\n", counter);
				}
			} else if(*nFOA > region) {//early exit: region is in the middle
               printf("middle\n");
				*regionPtr = *nFOA;
				*nFOA = region;
				justLooping = 1;
				counter++;
				counter++; //this works, trust me
				nFOA = *nFOA;
			}
		}
       printf("counter: %d\n", counter);
		//detect if slab is now empty
		if(++counter == numObjects) {
    printf("slab empty counter: %d numO: %d slabsize: %d\n", counter, numObjects, slabSize);
			ptr3[slabIdx]=ptr[slabIdx]; //firstFreeObj = firstObj
			memset(ptr[slabIdx], 0, slabSize); //all zero'd out
			ptr2[slabIdx]=0; //currentSlabObjSize = 0
			break;
		}
        if (lastFree == 0){
           if(*nFOA != 0)
            nFOA = *nFOA;//traverse dat linked list
		   if(justLooping==1 && *nFOA == NULL) {
		      	break;
		   }
        }
        else break;
	}
	
	/*//Next, we need to update the first free obj ptr while checking for if the whole slab
	//is now empty
	int empty = 0; int firstEmpty=0;
	for (i=0; i<(slabSize/ptr2[x]); i++){  //slabsize/ptr2[x] is # objects in this slab
	  if (ptr1[x] + i*ptr2[x]= ptr1[x] + (i+1)*ptr2[x]) {  //If the value of the object is the address of the next object...
		 if (firstEmpty=0){ // Then this entry is the first empty entry
			firstEmpty=i;
		 }
		 empty++;
	  }
	}*/

	for(i=0; i<1024; i++) {
		printf("%d ", ((int*)alloc_array[alloc_id])[i+48/4]);
	}
	puts("");
	
	printf("after loop, ptr3[%d]=%d\n", slabIdx, ptr3[slabIdx]);
	
	printf("counter = %d\n", counter);
	printf("numObjects = %d\n", numObjects);
	
	}