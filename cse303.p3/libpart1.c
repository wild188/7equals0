#include <stdio.h>
//Billy
#include <stdlib.h>
#include "support.h"
#include <string.h>
#include <ctype.h>

// hello() - print some output so we know the shared object loaded.
//
// The function signature takes an untyped parameter, and returns an untyped
// parameter.  In this way, the signature *could* support any behavior, by
// passing in a struct and returning a struct.
void *hello(void *input) {
    printf("hello from a .so\n");
    return NULL;
}

//Billys shit dont know how to test this
void * ucase(void * input){
    
    printf("Billy says hello!\n");
    
    //This could cause an error if input isnt a team structure
    struct team_t * oldTeam = (struct team_t *)input;

    struct team_t * newTeam = malloc(sizeof(struct team_t));
    
    //copying name1
    char * temp = malloc(sizeof(oldTeam->name1));
    *temp = *oldTeam->name1;
    newTeam->name1 = malloc(*temp);
    int i = 0;
    for(i = 0; i < strlen(temp); i++){
        newTeam->name1[i] = toupper(temp[i]);
    }
    free(temp);

    //copying email1
    temp = malloc(sizeof(oldTeam->email1));
    *temp = *oldTeam->email1;
    newTeam->email1 = malloc(*temp);
    i = 0;
    for(i = 0; i < strlen(temp); i++){
        newTeam->email1[i] = toupper(temp[i]);
    }
    free(temp);

    //copying name2
    temp = malloc(sizeof(oldTeam->name2));
    *temp = *oldTeam->name2;
    newTeam->name2 = malloc(*temp);
    i = 0;
    for(i = 0; i < strlen(temp); i++){
        newTeam->name2[i] = toupper(temp[i]);
    }
    free(temp);

    //copying email2
    temp = malloc(sizeof(oldTeam->email2));
    *temp = *oldTeam->email2;
    newTeam->email2 = malloc(*temp);
    i = 0;
    for(i = 0; i < strlen(temp); i++){
        newTeam->email2[i] = toupper(temp[i]);
    }
    free(temp);

    free(newTeam);
    return NULL;
}
