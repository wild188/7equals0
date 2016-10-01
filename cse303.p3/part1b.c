#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "support.h"
//Billy
#include <dlfcn.h>

/*
 * load_and_invoke() - load the given .so and execute the specified function
 */

typedef void* (*function)();

void load_and_invoke(char *libname, char *funcname) {
    /* TODO: complete this function */
    int mode = RTLD_LAZY; //I dont know what the mode should be
    void * lib = dlopen(libname, mode);
    if(lib == NULL){
        printf("Failed to load library with path %s\n", libname);
        printf("Error: %s\n", dlerror());
        exit(1);
    }

    function result;

    //gets the function and stores it in result; 
    *(void**)(&result) = dlsym(lib, funcname); // what the balls???
    if(result == NULL){
        printf("We might have failed to run %s in %s\n", funcname, libname);
    }

    result(NULL); // calling function with NULL as a paramater. 

    dlclose(lib);
}

/*
 * help() - Print a help message.
 */
void help(char *progname) {
    printf("Usage: %s [OPTIONS]\n", progname);
    printf("Load the given .so and run the requested function from that .so\n");
    printf("  -l [string] The name of the .so to load\n");
    printf("  -f [string] The name of the function within that .so to run\n");
}

/*
 * main() - The main routine parses arguments and invokes hello
 */
int main(int argc, char **argv) {
    /* for getopt */
    long opt;

    /* run a student name check */
    check_team(argv[0]);

    /* parse the command-line options.  For this program, we only support  */
    /* the parameterless 'h' option, for getting help on program usage. */
    while ((opt = getopt(argc, argv, "h")) != -1) {
        switch(opt) {
          case 'h': help(argv[0]); break;
        }
    }

    /* call load_and_invoke() to run the given function of the given library */
    //~/Programs/cse303/project3/7equals0/cse303.p3/obj64/libpart1.so
    ///home/billy/Programs/cse303/project3/7equals0/cse303.p3/obj64/libpart1.so
    
    load_and_invoke("./libpart1.so", "hello"); //we have to get eh library path

    exit(0);
}
