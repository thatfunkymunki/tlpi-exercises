/*
 * The tee command reads its standard input until end-of-file,
 * writing a copy of the input to the standard output and to the file
 * named in its command-line argument. (We show an example of the use
 * of this command when we discuss FIFOs in Section 44.7.) Implement 
 * tee using I/O system calls. By default, tee overwrites any existing
 * file with the given name. Implement the -a command-line option 
 * (tee -a file), which causes tee to append text to the end of a file
 * if it already exists. (Refer to Appendix B for a description of the
 * getopt() function, which can be used to parse command-line options.)
 */
#include "tlpi_hdr.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFSIZE 100
char buffer[BUFSIZE];
void usage(void);

int main (int argc, char** argv){
  Boolean append = FALSE;

  int opt=0;
  while((opt = getopt(argc, argv, ":a"))!=-1){
    switch(opt){
      case 'a': append = TRUE; break;
      case '?': usage(); break;
    }

  }
  if(optind >= argc||argc<2){
    usage();
  }
  

  int fd = open(argv[optind], O_WRONLY|O_CREAT | (append ? O_APPEND : O_TRUNC), S_IRUSR|S_IWUSR);
  if(fd == -1){
    errExit("open");
  }

  ssize_t num_read = 0;
  while((num_read = read(STDIN_FILENO, buffer,BUFSIZE)) >0){
    if(write(STDOUT_FILENO,buffer,num_read) == -1){
      errExit("failed STDOUT");
    }
    if(write(fd,buffer,num_read) == -1){
      errExit("failed file write");
    }
  }
  if(num_read ==-1){
    errExit("failed STDIN");
  }

  return 0;
}
void usage(){
  printf("tee [-a] <filename>\n");
  exit(1);
}
