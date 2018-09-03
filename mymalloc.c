#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mymalloc.h"
static char memory[5000];
int isEmpty = 1;
/*
    Overwrites malloc and intitializes the memory array if this is
    the first time malloc is being called in the client file.
*/
/*
void * mymalloc(size_t size, char* file, int line) {
    if (isEmpty){
        initialize();
        isEmpty = 0;
    }
    void *ptr = allocateBlock((int)size);
    if (ptr == NULL){
       fprintf(stderr, "Error in %s at line %d: not enough memory\n", __FILE__, __LINE__ );
    }
    return ptr;
}
*/
//    Creates an unallocated block that covers the entire memory array.
void initialize(){
    char *p;
    p = &memory[0];
    *((short*)p) = -5000;
}
void *allocateBlock (int size){
    char * curr = memory;
    int spaceReq = size + 2;
    int index = 0;
    int i;
    i = 0;
    //too small or too big
    if (size < 1 || size > 4998){
       return NULL;
    }
    do{
        short *blockSize = (short*)(curr);
        short savedBlockSize = *blockSize * -1;
        if (savedBlockSize < 0){
            savedBlockSize = savedBlockSize * -1;
        }
        int margin = savedBlockSize - spaceReq;
        if ((*blockSize < 0) && (margin == 0 || margin > 2)){
           short *p;
            //sets size of allocated space
            p = (short*)curr;
            *p = spaceReq;
            //sets size of new unallocated block if block is split
            if (spaceReq != savedBlockSize){
                p = (short*)(curr + spaceReq);
                *p = (savedBlockSize - spaceReq) * -1; //negative because unallocated
            }
            return (curr + 2);
        }
        else{
            index = index + savedBlockSize;
            curr = &memory[index];
            i++;
        }
    } while (index < 5000);
    return NULL;
}

void * mymalloc(size_t size, char* file, int line) {
    if (isEmpty){
        initialize();
        isEmpty = 0;
    }
    void *ptr = allocateBlock((int)size);
    if (ptr == NULL){
       fprintf(stderr, "Error in %s at line %d: not enough memory\n", __FILE__, __LINE__ );
    }
    return ptr;
}

void myfree(void * ptr, char* file, int line){
    char * curr = memory;
    char * prev = memory;
    short* currentSize = NULL;
    int index = 0;
    int blockNotFound = 1;
    if (ptr < (void*)&memory[0] || ptr > (void*)&memory[4997]){
        fprintf(stderr, "Error in %s at line %d\n",__FILE__, __LINE__ );
        blockNotFound = 0;
    }
    while (index < 5000 && blockNotFound){
        if (ptr == curr + 2){
            currentSize = (short*)curr;    
            if(*currentSize < 0){           //already free
                //do nothing
                fprintf(stderr, "Error in %s at line %d\n", __FILE__, __LINE__ );
            }
            else if (isFreeBlockBefore(curr, prev) && isFreeBlockBehind(index, curr)){
                short* prevSize = (short*)prev;
                char *next = &memory[index + *currentSize];
                short *nextSize = (short*)next;
                *prevSize = *prevSize - *currentSize + *nextSize;
            }
            else if (isFreeBlockBefore(curr, prev)){
                //increases size of previous block and keeps negative
                short* prevSize = (short*)prev;
                *prevSize = *prevSize - *currentSize;
            }
            else if (isFreeBlockBehind(index, curr)){
                //increases size of current block and makes negative
                char *next = &memory[index + *currentSize];
                short *nextSize = (short*)next;
                *currentSize = *nextSize - *currentSize;
            }
            else{
                *currentSize = *currentSize * -1;
            }
            blockNotFound = 0;
        }
        else {
            currentSize = (short*)curr;
            prev = curr;
            if (*currentSize < 0){
                index = index + (*currentSize * -1);
            }
            else{
                index = index + *currentSize;
            }
            curr = &memory[index];
        }
    }
    if (blockNotFound == 1){
        fprintf(stderr, "Error in %s at line %d\n", __FILE__, __LINE__ );
    }
}
/*
    Checks to see if the current block has an unallocated block before it. For
    use in the myfree function.
        Returns
            1 if free block is before current block
            0 otherwise
*/
int isFreeBlockBefore(char * curr, char * prev){
    short* prevSize = (short*)prev;
    //if first block or prev is allocated
    if (curr == prev && *prevSize > 0){
        return 0;
    }
    else if (*prevSize > 0){
        return 0;
    }
    else {
        return 1;
    }
}
int isFreeBlockBehind(int index, char * curr){
    short *currentSize = (short*)curr;
    char *next = &memory[index + *currentSize];
    short *nextSize = (short*)next;  
    if (((index + *currentSize) == 5000) || (*nextSize > 0)){
        return 0;
    }
    else {
        return 1;
    }
}
