#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <dlfcn.h>

/*
 * We aren't providing much code here.  You'll need to implement your own
 * malloc() and free(), as well as any constructors or destructors for your
 * library.
 */

/*
 * Declarations for the functions in list.c, so that we don't need an
 * extra header file.
 */
int map_insert(uintptr_t pointer, char *module, char *line);
int map_remove(uintptr_t pointer);
int map_count();
void map_dump();

/* TODO: Your code goes here */

int unfreeBlocks; //To count allocations and frees

//Declaring and mapping constructor and deconstructor functions
void OpenMemoryManagement()__attribute__((constructor)); 
void CloseMemoryManagement()__attribute__((destructor));

void OpenMemoryManagement() { // This is the constructor
    unfreeBlocks = 0;
}

void CloseMemoryManagement(){ // This is the deconstructor
    printf("%i unfreed blocks of memory.\n", unfreeBlocks);
}

//a wrapper function for the real malloc
static void * ogMalloc(size_t size){
    void * (*realMalloc)(size_t) = dlsym(RTLD_NEXT, "malloc");
    return realMalloc(size);
}

//wrapper function for the real OG free
static void ogFree(void * space){
    void (*realFree)(void *) = dlsym(RTLD_NEXT, "free");
    realFree(space);
}

//should intercept calls to malloc
void * malloc(size_t size){
    unfreeBlocks++;
    return ogMalloc(size);
}

//should intercept calls to free
void free(void * space){
    unfreeBlocks--;
    return ogFree(space);
}