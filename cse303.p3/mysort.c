#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * sort_file() - read a file, sort its lines, and output them.  reverse and
 *               unique parameters are booleans to affect how the lines are
 *               sorted, and which lines are output.
 */

int cmpfunc(const void *a, const void *b){
  const char *one = *(const char **)a;
  const char *two = *(const char **)b;
  return strcmp(one, two);

}

int cmpfuncR(const void *a, const void *b){
  const char *one = *(const char **)a;
  const char *two = *(const char **)b;
  return strcmp(one, two) * -1;

}


void sort_file(char *filename, int unique, int reverse) {
  FILE *in = fopen(filename, "r");
  char *array[1000000];
  char temp[1024];
  char ch;
  int charCount = 0;
  int index = 0;
  int lineCount = 0;
  int i = 0;
  int x;
  while((ch = fgetc(in)) != EOF){
    if(ch == '\n'){
      temp[charCount] = '\n';
      i = 0;
      array[index] = (char *)malloc(sizeof(char) * (charCount + 1));
      while(i != (charCount + 1)){
	array[index][i] = temp[i];
	i++;
      } 
      array[index][i] = '\0';
      index++;
      lineCount++;
      charCount = 0;
      for(x = 0; x < 1024; x++){
	temp[x] = '\0';
      }
      continue;
    }
    temp[charCount] = ch;
    charCount++;
  }
  int j = 0;      
  if(reverse){
    qsort(array, lineCount, sizeof(char *), cmpfuncR);
  }
  else{
    qsort(array, lineCount, sizeof(char *), cmpfunc);
  }
  for(; j < lineCount; j++){
    if(strstr(array[j], "\n!") != NULL){
      array[j][strlen(array[j] - 1)] = '\0';
    }
    if((j - 1) >= 0 && unique){
      if(!strcmp(array[j], array[j-1])){
	continue;
      }
    }
    printf("%s", array[j]);
  }
  for(j = 0; j < lineCount; j++){
    free(array[j]);
  }
  fclose(in);
}


/*
 * help() - Print a help message.
 */
void help(char *progname) {
    printf("Usage: %s [OPTIONS] FILE\n", progname);
    printf("Sort the lines of FILE and print them to STDOUT\n");
    printf("  -r    sort in reverse\n");
    printf("  -u    only print unique lines\n");
}

/*
 * main() - The main routine parses arguments and dispatches to the
 *          task-specific code.
 */
int main(int argc, char **argv) {
    /* for getopt */
    long opt;
    int reverseFlag = 0;
    int uniqueFlag = 0;

    /* ensure the student name is filled out */
    //check_student(argv[0]);

    /* parse the command-line options.  They are 'r' for reversing the */
    /* output, and 'u' for only printing unique strings.  'h' is also */
    /* supported. */
    /* TODO: parse the arguments correctly */
    while ((opt = getopt(argc, argv, "huru:")) != -1) {
        switch(opt) {
	case 'h': help(argv[0]); break;
	case 'r': reverseFlag = 1; break;
	case 'u': uniqueFlag = 1; break; 
        }
    }

    /* TODO: fix this invocation */
    sort_file(argv[argc - 1], uniqueFlag, reverseFlag);
}
