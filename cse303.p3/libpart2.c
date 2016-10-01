#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

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

void * malloc(size_t size){
    unfreeBlocks++;
    //call real malloc
}

void free(void * space){
    unfreeBlocks--;
    //call real free
}