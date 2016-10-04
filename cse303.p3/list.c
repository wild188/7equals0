#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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

  if(find_node(pointer)){
    return 0; //we already had the allocation in our allocation information
  }
  
  map_node_t * curr = alloc_info;
  if(curr != NULL){
    while(curr->next != NULL){
      curr = curr->next;
    }
  }
  
  map_node_t * newnode = malloc(sizeof(map_node_t));
  //assigning
  curr->next = newnode;

  newnode->allocated_pointer = pointer;
  //what is a molule, line, call site, or program counter
  //the following lines are most definately wrong
  newnode->call_site = module;
  newnode->program_counter = line;
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
 
int map_remove(uintptr_t pointer) {
  
  map_node_t* curr = alloc_info;
  if(curr != NULL){
    while(curr->next != NULL){
      if(curr->next->allocated_pointer == pointer){
        map_node_t * target = curr->next;
        curr->next = curr->next->next;
        free(target);
        return 1;
      }
    }
  }

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
  /* TODO: complete this code */
  return -1;
}

/*
 * dump() - output the contents of the list
 */
void map_dump() {
  
  map_node_t* curr = alloc_info;
  printf("Map dump %p\n", curr);
  int i = 0;
  while (curr) {
    curr = &(alloc_info[i]);
    i++;
    printf("  0x%x allocated by %s::%s", (uint)curr->allocated_pointer, curr->call_site, curr->program_counter);
  }
}
