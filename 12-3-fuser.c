/*Exercise 12-3
 * Write a program that lists all processes that have a particular file pathname open.
 * This can be achieved by inspecting the contents of all of the /proc/PID/fd/\* symbolic
 * links. This will require nested loops employing readdir(3) to scan all /proc/PID
 * directories, and then the contents of all /proc/PID/fd entries within each /proc/PID
 * directory. To read the contents of a /proc/PID/fd/n symbolic link requires the use
 * of readlink(), described in Section 18.5.
 */
 
 
#include "tlpi_hdr.h"
#include "libprochelper.h"
#include <dirent.h>
#include <ctype.h>
#include <string.h>
 

int main(int argc, char** argv){

  DIR *proc;
  struct dirent *dp;
  
  proc = opendir("/proc/");
  if(proc == NULL){
    errExit("failed opening /proc/");
  }
  do{
    errno=0;
    dp = readdir(proc);
    if(dp!=NULL){
      
      if(dp->d_type == DT_DIR || dp->d_type == DT_LNK){ //walk all dirs and symlinks
        if( !string_is_number(dp->d_name) ) { //only look through PID dirs
          continue;
        }
      }
    }
  } while(dp!=NULL);
  if(errno !=0){
    errExit("readdir");
  }
  
  return 0;
}