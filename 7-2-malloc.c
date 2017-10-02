/* Exercise 7-2
 * (Advanced) Implement malloc() and free()
 *
 * Sources for reference: 
 * http://tharikasblogs.blogspot.com/p/how-to-write-your-own-malloc-and-free.html
 * https://danluu.com/malloc-tutorial/ 
 */
#include "tlpi_hdr.h"
//need to create a metadata struct for each region

typedef enum st { FREE, USED } status;
typedef struct blk{
  size_t size;
  struct blk *next;
  status free;
} block;

void *_malloc(size_t size);
void _free(void *ptr);
block *find_free_block(size_t size);
block *allocate_space(size_t size);
block *get_block(void *ptr);



//top of heap linked list
block *head, *last = NULL;


block *find_free_block(size_t size){ //important to track the ptr to the last block as well as the one you return
  block *current = head;
  while(current !=NULL && !(current->free == FREE && current->size >= size)){ //continue down list until a free block of appropriate size is found
    last=current;
    current=current->next;
  }
  return current;
}

block *allocate_space(size_t size){
  block *new;
  new = sbrk(0);
  void *request = sbrk(size+sizeof(block)); //push break up enough for size and metadata
  if(request == (void *) -1){ //sbrk returns -1 if failed
    return NULL;
  }
  
  if(last != NULL){//should only be null on the first call to malloc
    last->next = new; //add new block to list
  }
  new->size = size;
  new->next = NULL;
  new->free = USED;
  return new;
}

void *_malloc(size_t size){
  block *new;
  
  if(size<=0){
    return NULL;
  }
  
  if(head == NULL){ //first call to malloc in program
    new = allocate_space(size);
    if(new == NULL){ //couldn't allocate space
      return NULL;
    }
    head = new;
    last = new;
  }
  else{
    new = find_free_block(size);
    if(new == NULL){ //couldn't find any free blocks of appropriate size
      new = allocate_space(size);
      if(new==NULL){
        return NULL;
      }
    }
    else{
      new->free = USED;
    }
  }
  return new+1; //leave space for a block struct
}

block *get_block(void *ptr){
  return (block *)ptr-1;
  
}

void _free(void *ptr){
  //very simple free(), only marks a block as free (doesn't remove it from the pool)
  if(ptr == NULL){
    return;
  }
  block *ptr_block = get_block(ptr);
  ptr_block->free = FREE;
}

int main(int argc, char** argv){
  //verify our malloc works

  printf("program break: %p\n", sbrk(0));
  
  char **newstr = _malloc(sizeof(char *)*8);
  for (int i = 0; i < 8; i++){
    newstr[i]= _malloc(sizeof(char)*8);
    sprintf(newstr[i],"%d%d%d%d%d%d%d",i,i,i,i,i,i,i);
  }
  for (int i = 0; i < 8; i++){
    printf("%p: %s\n",(void *)&newstr[i], newstr[i]);
    _free(newstr[i]);
  }
  //make sure itworks again after freeing everything else
  for (int i = 0; i < 8; i++){
    newstr[i]= _malloc(sizeof(char)*8);
    sprintf(newstr[i],"%d%d%d%d%d%d%d",i,i,i,i,i,i,i);
  }
  for (int i = 0; i < 8; i++){
    printf("%p: %s\n",(void *)&newstr[i], newstr[i]);
    _free(newstr[i]);
  }
  
  return 0;
}
