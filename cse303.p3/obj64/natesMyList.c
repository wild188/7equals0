#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
//#include "support.h"

/*
 * list_task() - read from stdin, determine the appropriate behavior, and
 *               perform it.
 */
//gcc -Wall -Werror -o mylist2 mylist.c  ./libpart2.so -ldl

struct LL {
  int value;
  struct LL *head;
  struct LL *tail;
};

struct LL *headNode;

void deleteMem(){
  struct LL *currentNode = headNode;
  while(currentNode->tail != NULL){
    struct LL *temp = currentNode;
    currentNode = currentNode->tail;
    free(temp);
  }
  if(currentNode != NULL){
    free(currentNode);
  }
}

void insert(int num){
  struct LL *newNode = malloc(sizeof(struct LL));
  struct LL *currentNode = headNode;
  newNode->value = num;
  while((currentNode->tail != NULL) && (currentNode->value <= num)){
    currentNode = currentNode->tail;
  }
  if(currentNode->value > num){
    struct LL *temp = currentNode->head;
    temp->tail = newNode;
    newNode->head = temp;
    newNode->tail = currentNode;
    currentNode->head = newNode;
  }
  else{
    currentNode->tail = newNode;
    newNode->head = currentNode;
  }
}

void removeNum(int num){
  struct LL *currentNode = headNode->tail;
  while(currentNode->value <= num){
    if(currentNode->value == num){
      if(currentNode->tail == NULL){
	currentNode->head->tail = NULL;
	free(currentNode);
	return;
      }
      currentNode->head->tail = currentNode->tail;
      currentNode->tail->head = currentNode->head;
      free(currentNode);
      return;
    }
    currentNode = currentNode->tail;
  }
  
}

void print(){
  if(headNode->tail == NULL){
    return;
  }
  struct LL *currentNode = headNode->tail;
  while(currentNode->tail != NULL){
    printf("%i-->", currentNode->value);
    currentNode = currentNode->tail;
  }
  printf("%i\n", currentNode->value);
}

void list_task(void) {
  headNode = malloc(sizeof(struct LL));
  headNode->value = '\0';
  char command[10];
  char ch;
  int num;
  printf("enter i followed by a number to insert, r followed by a number to remove, p to print, and x to exit\n");
  while(1){
    fgets(command, 10, stdin);
    sscanf(command, "%c %i", &ch, &num);
    switch(ch){
    case 'i':
      insert(num);
      break;
    case 'r':
      removeNum(num);
      break;
    case 'p':
      print();
      break;
    case 'x':
      deleteMem();
      return;
      break;
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
