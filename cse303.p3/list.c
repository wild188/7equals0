#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

//#include "libpart2.c"

/*
 * Note: 'uintptr_t' is a special type of unsigned integer that is guaranteed
 *       to be the same size as pointers.  This is the preferred way to cast
 *       between pointers and integers in code that could be compiled in
 *       32-bit or 64-bit mode.
 */

/*
 * A type for nodes in a linked list.  The linked list will be used to map
 * pointers to strings, so we can save information about the state of the
 * program each time a pointer was returned by malloc()
 */
typedef struct map_node
{
  uintptr_t allocated_pointer;
  char      *call_site;
  char      *program_counter;
  struct map_node * next;
} map_node_t;

/*
 * A list, based on map_node_t
 *
 * NB: The list is static, so that its name isn't visible from other .o files
 */
static map_node_t* alloc_info;

//return the index of the map node -1 if not found
map_node_t* find_node(uintptr_t target){
  map_node_t* curr = alloc_info;
  while(curr != NULL){
    if(curr->allocated_pointer == target){
      printf("Im the worlds greatest detecive\n");
      return curr;
    }
    curr = curr->next;
  }
  return NULL;
}

/*
 * insert() - when malloc() is called, your interpositioning library
 *            should use this to store the pointer that malloc returned,
 *            along with the module name and line of code that
 *            backtrace() returned.  Return 1 if the pointer was
 *            successfully added, and 0 if the pointer was already in the
 *            map.
 */
 
int map_insert(uintptr_t pointer, char *module, char *line) {

  printf("inserting map node\n");

  if(find_node(pointer)){
    printf("returned 0");
    return 0; //we already had the allocation in our allocation information
  }
  map_node_t * newnode = (map_node_t *)malloc(sizeof(map_node_t));

  newnode->allocated_pointer = pointer;
  char modCpy[100];
  char lineCpy[100];
  //the malloced newnode now gets the values of module and line copied into call_site and program_counter
  strcpy(modCpy, module);
  strcpy(lineCpy, line);

  newnode->call_site = modCpy;
  newnode->program_counter = lineCpy;

  map_node_t * curr = alloc_info;

  int i = 0;

  if(curr != NULL){
    while(curr->next != NULL){
      curr = curr->next;
      i++;
    }
  }else{
    curr = newnode;
    alloc_info = curr; //why do I need this line???
    //printf("newNode %p  == list %p\n", newNode, alloc_info);
    //printf("  0x%x allocated by %s::%s\n", (uint)alloc_info->allocated_pointer, alloc_info->call_site, alloc_info->program_counter);
    //printf("  0x%x allocated by %s::%s\n", (uint)newNode->allocated_pointer, curr->call_site, curr->program_counter);
    return 1;
  }
  
  
  //assigning
  curr->next = newnode;
  printf("  0x%x allocated by %s::%s\n", (uint)newnode->allocated_pointer, newnode->call_site, newnode->program_counter);


  return 1;
}

/*
 * remove() - when free() is called, your interpositioning library should use
 *            this to remove the pointer and its strings from the list
 *            declared above.  In this way, you'll be able to track, at all
 *            times, which allocations are outstanding, and what line of code
 *            caused those allocations.  Return 1 if the remove was
 *            successful, and 0 if the pointer was already removed from the
 *            map (which would suggest a double-free).
 */
 
void map_dump();

int map_remove(uintptr_t pointer) {
  
  printf("removinging map node\n");

  map_node_t* curr = alloc_info;
  if(curr != NULL){
    if(curr->allocated_pointer == pointer){
      alloc_info = curr->next;
      free(curr);
      printf("first ret");
      map_dump();
      return 1;
    }
    while(curr->next != NULL){
      if(curr->next->allocated_pointer == pointer){
        map_node_t * target = curr->next;
        curr->next = curr->next->next;
        free(target);
        printf("second ret");
        map_dump();
        return 1;
      }
      curr = curr->next;
    }
  }
  printf("third ret");
  map_dump();
  return 0;
/*
  map_node_t * space = find_node(pointer);
  if(space){
    
  }else{ //the pointer wasn't in our allocation info
    return 0;
  }
  return 0;
*/
}

/*
 * count() - return the number of elements in the map.  This can indicate
 *           that there are un-freed allocations (memory leaks).
 */
int map_count() {
  map_node_t * temp = alloc_info;
  int count = 0;
  if(temp == NULL){
    return 0;
  }
  while(temp->next != NULL){
    temp = temp->next;
    count++;
  }
  return count;
}

/*
 * dump() - output the contents of the list
 */
void map_dump() {
  
  map_node_t* temp = alloc_info;
  printf("Map dump %p\n", temp);
  int i = 0;
  if(temp == NULL){
    printf("No allocations remain\n");
    return;
  }
  while (temp->next != NULL) {
    temp = temp->next;
    printf("  0x%x allocated by %s::%s\n", (uint)temp->allocated_pointer, temp->call_site, temp->program_counter);
  }
}
