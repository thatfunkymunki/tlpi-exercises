/* EXERCISE 5-2
 * Write a program that opens an existing file for
 * writing with the O_APPEND flag, and then seeks
 * to the beginning of the file before writing some
 * data. Where does the data appear in the file? Why?
 */

#include <stdio.h>
#include "tlpi_hdr.h"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

void usage(void);

int main(int argc, char** argv){
  char appendtext[] = "123456 TEXT TO APPEND";
  if (argc!=2){
    usage();
  }

  int dest = open(argv[1], O_WRONLY|O_APPEND);
  if(dest==-1){
    errExit("failed opening dest");
  }

  lseek(dest,0,SEEK_SET);

  write(dest,appendtext, strlen(appendtext));
  return 0;
}
void usage(void){
  printf("append <destfile>\n");
  exit(1);
}

