//gcc -Wall -Werror -o mylist2 mylist.c  ./libpart2.so -ldl

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
//#include "support.h"
#include <ctype.h>
#include <string.h>

/*
 * list_task() - read from stdin, determine the appropriate behavior, and
 *               perform it.
 */

struct listNode{
  int value;
  struct listNode * prev;
  struct listNode * next;
} head;

//struct listNode * head;

void list_task(void) {
    /* TODO: Complete this function */

  //head = malloc(sizeof(listNode));
  head.prev = NULL;
  head.next = NULL;
  
  //FILE *input = stdin;
  
  while(1){
    int tempVal;
    char tempComand;    
    
    char input[100];
    char curChar;
    int i = 0;
    while((curChar = getchar())){
      if(curChar == '\n'){
	input[i] = '\0';
	break;
      }
      input[i] = curChar;
      i++;
    }

    //printf("%s\n", input);
    sscanf(input, "%c %i", &tempComand, &tempVal);
        
    //printf(" Read in comand: %c, value: %i", tempComand, tempVal);
    
    if(tempComand == 'p'){
      struct listNode * curNode = &head;
      if(curNode->next != NULL){
	curNode = curNode->next;
      }else{
	continue;
      }
      
      printf("%i", curNode->value);
      
      while(curNode->next != NULL){
	curNode = curNode->next;
	printf("-->%i", curNode->value);
	
      }
      printf("\n");
      
    }else if(tempComand == 'x'){
      /*struct listNode * curNode  = head.next;
      struct listNode * nextNode;
      while(curNode != NULL){
	      nextNode = curNode->next;
	      free(curNode); //edit to create bad code
	      curNode = nextNode;
      }
      //exit program*/
      return;
    }else if(tempComand == 'i'){
      struct listNode * curNode = &head;
      while(curNode->next != NULL && curNode->next->value <= tempVal){
	curNode = curNode->next;
      }
      struct listNode * newNode = malloc(sizeof(struct listNode));
      newNode->value = tempVal;
      newNode->next = curNode->next;
      newNode->prev = curNode;
      curNode->next = newNode;

      //printf("    Added new node with val %i  ", newNode->value);
      
    }else if(tempComand == 'r'){
      struct listNode * curNode = &head;
      struct listNode * prevNode;
      
      while(curNode->next != NULL){
	prevNode = curNode;
	curNode = curNode->next;
	if(curNode->value == tempVal){
	  if(curNode->next != NULL){
	    prevNode->next = curNode->next;
	    
	  }else{
	    prevNode->next = NULL;
	  }
	  if(curNode->next != NULL){
	    curNode->next->prev = prevNode;
	  }
	  free(curNode);
	  break;
	}
      }
      //remove
    }else{
      printf("Comand %c not recognized\n", tempComand);
    }
  }

}

/*
 * help() - Print a help message.
 */
void help(char *progname) {
    printf("Usage: %s\n", progname);
    printf("Run an interactive list manipulation program\n");
    printf("Commands: [i]insert, [r]emove, [p]rint, e[x]it\n");
    printf("  (Commands require an integer argument)\n");
}

/*
 * main() - The main routine parses arguments and dispatches to the
 *          task-specific code.
 */
int main(int argc, char **argv) {
    /* for getopt */
    long opt;

    /* run a student name check */
    //check_student(argv[0]);

    /*
     * parse the command-line options.  For this program, we only support the
     * parameterless 'h' option, for getting help on program usage.
     */
    while ((opt = getopt(argc, argv, "h")) != -1) {
        switch(opt) {
          case 'h': help(argv[0]); break;
        }
    }

    /* run the list program */
    list_task();
}
