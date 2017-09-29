/* Exercise 7-4
 * (Advanced) Implement malloc() and free()
 *
 * Sources for reference: 
 * https://danluu.com/malloc-tutorial/ 
 * http://tharikasblogs.blogspot.com/p/how-to-write-your-own-malloc-and-free.html
 */
#include "tlpi_hdr.h"

//prototypes
void *_malloc(size_t size);
void _free(void *ptr);
enum status { FREE, USED };
struct block *find_free_block(struct block **last, size_t size);
struct block *allocate_space(struct block *last, size_t size);
struct block *get_block(void *ptr);



//top of heap linked list
void *head = NULL;

//need to create a metadata struct for each region
struct block{
  size_t size;
  struct block *next;
  enum status free;
};

struct block *find_free_block(struct block **last, size_t size){
  struct block *current = head;
  while(current !=NULL && !(current->free == FREE && current->size >= size)){
    *last=current;
    current=current->next;
  }
  return current;
}

struct block *allocate_space(struct block *last, size_t size){
  struct block *new;
  new = sbrk(0);
  void *request = sbrk(size+sizeof(struct block));
  if(request == (void *) -1){
    return NULL;
  }
  
  if(last != NULL){
    last->next = new;
  }
  new->size = size;
  new->next = NULL;
  new->free = USED;
  return new;
}

void *_malloc(size_t size){
  struct block *new;
  
  if(size<=0){
    return NULL;
  }
  
  if(head == NULL){
    new = allocate_space(NULL,size);
    if(new == NULL){
      return NULL;
    }
    head = new;
  }
  else{
    struct block *last = head;
    new = find_free_block(&last, size);
    if(new == NULL){
      new = allocate_space(last,size);
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
struct block *get_block(void *ptr){
  return (struct block *)ptr-1;
  
}

void _free(void *ptr){
  if(ptr == NULL){
    return;
  }
  struct block *ptr_block = get_block(ptr);
  ptr_block->free = FREE;
}

int main(int argc, char** argv){

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
  
  return 0;
}
