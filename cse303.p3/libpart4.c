#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <dlfcn.h>
//#include "list.c"
#include <execinfo.h>
#include <string.h>
#include <stdlib.h>

void CheckPipe()__attribute__((constructor));

void CheckPipe(){
    void **calls
    int size = 100;
    int ret = backtrace(calls, size);
    int i;
    while(calls[i] != NULL){
        if(strstr(calls[i], 'printf') || strstr(calls[i], 'scanf'){
            int pageSize = sysconf(_SC_PAGE_SIZE);
            mprotect((void*)((size_t)&pageSize) & (pageSize - 1), pageSize, PROT_NONE);
        }
        i++;
    } 

}