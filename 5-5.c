/* EXERCISE 5-5
 * Write a program to verify that duplicated file descriptors share a file offset value
 * and open file status flags.
 *
 */
#include "tlpi_hdr.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

void usage(void);
void usage(void){
  printf("5-5 <filename>\n");
  exit(-1);
}

int main (int argc, char** argv){

  int file, dupe =0;
  //open a file supplied by arg
  if(argc!=2){
    usage();
  }
  if((file = open(argv[1],O_RDONLY))==-1){
    return -1;
  }

  //duplicate its descriptor
  dupe = dup(file);

  //verify same offset values
  if(lseek(file,0,SEEK_CUR) == lseek(dupe,0,SEEK_CUR)){
    printf("offsets are the same!\n");
  }
  else{
    printf("offsets are not the same!\n");
  }
  //verify same flags
  if(fcntl(file, F_GETFL) == fcntl(dupe, F_GETFL)){
    printf("flags are the same!\n");
  }
  else{
    printf("flags are not the same!\n");
  }


}
