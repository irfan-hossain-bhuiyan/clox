#include "memory.h"
#include <stdlib.h>
void* reallocate(void* pointer,size_t oldsize,size_t newsize){//It basically says not to new library
							      //function openly,You don't know when you
							      //need to edit it.
	if (newsize==0){
		free(pointer);
		return NULL;
	}
	void* result=realloc(pointer,newsize);
	if(result==NULL){exit(1);}
	return result;
}
