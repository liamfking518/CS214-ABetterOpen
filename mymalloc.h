#ifndef _MY_MALLOC_H
#define _MY_MALLOC_H

#define malloc(x) mymalloc((x), __FILE__, __LINE__)
#define free(x) myfree((x), __FILE__, __LINE__)

void* mymalloc(size_t, char*, int);
void myfree(void*, char*, int);

typedef struct _Node
{
	int* nodeID;
	int* size;
	struct _Node* next;
} Node;

#endif
