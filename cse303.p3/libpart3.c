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

#include <unistd.h>
#include <fcntl.h>
/*
 * We aren't providing much code here.  You'll need to implement your own
 * printf() and scanf(), as well as any constructors or destructors for your
 * library.
 */

/* TODO: Your code goes here */

static int theFinalCountDown;
static int evilMode;
static pid_t mypid;
static pid_t normalLib;

//Declaring and mapping constructor and deconstructor functions
void StartCountDown()__attribute__((constructor)); 
void Teminator()__attribute__((destructor));

//prototypes
int ogPrintf(const char * formatString, ...);

//Constructor
void StartCountDown(){
    theFinalCountDown = 4;
    evilMode = 0;
    mypid = 0;
    normalLib = getpid();
    ogPrintf("Fucking shit up!\n");
}

void Terminator(){
    ogPrintf("Turn down for what\n");
}

//printf wrapper function
int ogPrintf(const char * formatString, ...){
    va_list args;
    va_start(args, formatString);

    char * output;
    vasprintf(&output, formatString, args);

    int (*realPrintf)(const char *, ...) = dlsym(RTLD_NEXT, "printf");
    int out = realPrintf("%s", output);
    va_end(args);
    return out;
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

void engageDrEvil(){
    
    if(evilMode){
        //Dr evil is already engaged
        return;
    }else{
        evilMode = 1;


        //char data[100]={0};
        int com[2] ;
       // int nbytes;
        //char buf[100]={0};
        pipe(com);



        mypid = fork();
        if(mypid){
            //we are the evil child
            char * filename = "evil.txt";
            FILE * out = fopen(filename, "w");
            FILE * in = fdopen(com[0], "r");


            const char * test = getenv("PATH");

            if(0){
                //dup2(STDIN, EVILFILENAME);
            }else{
                
                
                
                close(com[1]); //closed writing end

                //dup2(com[0], out);
                
                ogPrintf("Created file\n");

            }   

            int status;
            char buf[1024];
            while(1){
                if(fgets(buf, 1024, in)){
                    fprintf(out, buf);
                    fflush(out);
                    fprintf(stdout, buf);
                    fflush(stdout);
                }
            }

            while(waitpid(normalLib, &status, WNOHANG | WUNTRACED)){
                if(WIFEXITED(status)){
                    //load exploit librarr
                    ogPrintf("Our parent has died and now we are an orphan!\n");
                    close(com[0]); //close reading end
                    exit(0);
                }else{
                    continue;
                }
            }
        }else{
            //we are the parent

            ogPrintf("We are the parent\n");

            close(com[0]); //closes reading end
            dup2(com[1], 1); //duping output to write pipe;
            //int a[] = {0, mypid};
            //pipe(a);
        }
    }
}

//what the fuck are we going to do with teh ...???

//int printf(__const char * __restrice __format, ...)

//int printf(__const char * format, ...){ 
//__fortify_function int printf (const char *__restrict __fmt, ...){

//int printf(const char * __fmt, ...){
int printf (const char *__restrict __fmt, ...){
    ogPrintf("Counting down: %i\n", theFinalCountDown);
    //const char * formatString = __fmt; //useless
    
    theFinalCountDown = theFinalCountDown - 1;
    
    //dealing with teh ...s
    va_list args;
    va_start(args, __fmt);

    if(theFinalCountDown < 0){
        //evil shit

        engageDrEvil();

        //old code
        /*
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
        */
    }

    char * output;
    vasprintf(&output, __fmt, args);

    int (*realPrintf)(const char *, ...) = dlsym(RTLD_NEXT, "printf");
    int out = realPrintf("%s", output);

    va_end(args);
    //ogPrintf(__fmt, args); // how do i pass the ...s?
    
    //not sure about this
    //va_end(args);

    return out; //Dont know what shoule be returned
}

int scanf(const char * fmt, ...){
    //dealing with the ...s
    theFinalCountDown--;
    if(theFinalCountDown < 0){
        engageDrEvil();
    }
    
    va_list args;
    va_start(args, fmt);
    va_end(args);
    int result = vscanf(fmt, args);//ogScanf(fmt, args);
    ogPrintf("Bullshit: "); //lack of proccessing creates infinite loop
    //va_end(args);
    return result;
}