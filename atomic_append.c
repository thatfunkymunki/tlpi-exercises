/* EXERCISE 5-3
 * This exercise is designed to demonstrate why the atomicity 
 * guaranteed by opening a file with the O_APPEND flag is necessary. 
 * Write a program that takes up to three command-line arguments:
 *      $ atomic_append <filename> <num-bytes> [x]
 * This program should open the specified filename (creating it if
 * necessary) and append <num-bytes> bytes to the file by using write()
 * to write a byte at a time. By default, the program should open the
 * file with the O_APPEND flag, but if a third command-line argument (x) 
 * is supplied, then the O_APPEND flag should be omitted, and instead the
 * program should perform an lseek(fd, 0, SEEK_END) call before each 
 * write(). Run two instances of this program at the same time without 
 * the x argument to write 1 million bytes to the same file:
 *      $ atomic_append f1 1000000 & atomic_append f1 1000000
 * Repeat the same steps, writing to a different file, but this time
 * specifying the x argument:
 *      $ atomic_append f2 1000000 x & atomic_append f2 1000000 x
 * List the sizes of the files f1 and f2 using ls -l and explain the 
 * difference.
 *
 */

#include <stdio.h>
#include "tlpi_hdr.h"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

void usage(void);

int main(int argc, char** argv){
  Boolean seek = FALSE;
  char x = 'x';


  if (argc<3||argc>4){
    usage();
  }
  else if (argc==4 && strcmp(argv[3],"x") == 0){
    seek = TRUE;
  }
  //printf("%s", (seek == TRUE)? "seek is true\n": "seek is false\n");
  int dest = open(argv[1], O_WRONLY|O_CREAT|((seek == FALSE) ? O_APPEND : 0) , S_IRUSR|S_IWUSR);
  if(dest==-1){
    errExit("failed opening dest");
  }
  for(int i = 0; i < atoi(argv[2]); i++){
    if(seek==TRUE){
      lseek(dest,0,SEEK_END);
    }

    write(dest,&x,1);
  }
  return 0;
}
void usage(void){
  printf("atomic_append <destfile> <num-bytes> [x]\n");
  exit(1);
}

