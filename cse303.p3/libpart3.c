#include <stdint.h>
#include <stddef.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h> //... wizardry
#include <dlfcn.h>

#include <sys/cdefs.h>
#include <stdio.h>

/*
 * We aren't providing much code here.  You'll need to implement your own
 * printf() and scanf(), as well as any constructors or destructors for your
 * library.
 */

/* TODO: Your code goes here */

static int theFinalCountDown;
static int childprocess;

//Declaring and mapping constructor and deconstructor functions
void StartCountDown()__attribute__((constructor)); 
void Teminator()__attribute__((destructor));

//prototypes
void ogPrintf(const char * formatString, ...);

//Constructor
void StartCountDown(){
    theFinalCountDown = 4;
    childprocess = 0;
    ogPrintf("Fucking shit up!\n");
}

void Terminator(){
    ogPrintf("Turn down for what\n");
}

//printf wrapper function
void ogPrintf(const char * formatString, ...){
    va_list args;
    va_start(args, formatString);
    void (*realPrintf)(const char *, ...) = dlsym(RTLD_NEXT, "printf");
    realPrintf(formatString, args);
    va_end(args);
    return;
}

int ogScanf(const char * fmt, ...){
    va_list args;
    va_start(args, fmt);
    int (*realScanf)(const char *, ...) = dlsym(RTLD_NEXT, "scanf");
    int result = realScanf(fmt, args); 
    va_end(args);
    return result; 
}

void evilPrintf(const char * formatString){
    //Balls
    ogPrintf("Evil shit\n");
}

//what the fuck are we going to do with teh ...???

//int printf(__const char * __restrice __format, ...)

//int printf(__const char * format, ...){ 
//__fortify_function int printf (const char *__restrict __fmt, ...){

int printf(const char * fmt, ...){
    ogPrintf("Counting down: %i\n", theFinalCountDown);
    const char * formatString = fmt; //useless
    
    theFinalCountDown = theFinalCountDown - 1;
    
    //dealing with teh ...s
    va_list args;
    va_start(args, fmt);

    if(theFinalCountDown < 0){
        //evil shit
        pid_t mypid = fork();
        if(mypid == 0){
            //childprocess
            evilPrintf(formatString);
            exit(0);
        }else{
            //Parent process
            int status;
            waitpid(mypid,&status, WNOHANG); //Not sure about the no hang option here
            if(status != 0){
                //This could be an issue
            }
        }
    }
    ogPrintf(formatString, args); // how do i pass the ...s?
    
    //not sure about this
    va_end(args);

    return 0; //Dont know what shoule be returned
}

int scanf(const char * fmt, ...){
    //dealing with the ...s
    va_list args;
    va_start(args, fmt);
    int result = ogScanf(fmt, args);
    ogPrintf("Bullshit: "); //lack of proccessing creates infinite loop
    va_end(args);
    return result;
}