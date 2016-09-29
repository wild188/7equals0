#include <stdio.h>

// hello() - print some output so we know the shared object loaded.
//
// The function signature takes an untyped parameter, and returns an untyped
// parameter.  In this way, the signature *could* support any behavior, by
// passing in a struct and returning a struct.
void *hello(void *input) {
    printf("hello from a .so\n");
    return NULL;
}

//Billys shit
void * ucase(void * input){
    struct team_t * newTeam = malloc(sizeOf(team_t);
    //how do we access unnames string literal fields in team_t struct?
    free(newTeam);
    return NULL;
}
