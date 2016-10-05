#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <dlfcn.h>
//#include "list.c"

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

//int unfreeBlocks; //To count allocations and frees

//to ingnore backtrace on child processes that might use malloc
static int childprocess;

//Declaring and mapping constructor and deconstructor functions
void OpenMemoryManagement()__attribute__((constructor)); 
void CloseMemoryManagement()__attribute__((destructor));

void OpenMemoryManagement() { // This is the constructor
    childprocess = 1;
    printf("created memory management lib\n");
    //unfreeBlocks = 0;
    childprocess = 0;
}

void CloseMemoryManagement(){ // This is the deconstructor
    childprocess = 1;
    int remainingBlocks = map_count();
    printf("%i unfreed blocks of memory.\n", remainingBlocks); //unfreeBlocks);
    if(1 || remainingBlocks != 0){
        map_dump();
    }
    childprocess = 0;
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
    void * pointer = ogMalloc(size);
    //need to get module and line pointers
    if(!childprocess){
        childprocess = 1;
        //unfreeBlocks++; //not sure where this line should be
        map_insert((uintptr_t)pointer, NULL, NULL);
        childprocess = 0;
    }
    return pointer;
}

//should intercept calls to free
void free(void * space){
    if(!childprocess){
        childprocess = 1;
        //unfreeBlocks--;
        map_remove((uintptr_t)space);
        childprocess = 0;
    }
    return ogFree(space);
}