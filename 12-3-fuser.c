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

void check_pid(const char *pid_s, const char *file);
void usage(const char *name);


void check_pid(const char *pid_s, const char *file){
  DIR *pid_dir;
  struct dirent *dp;
  char filename[256]= "/proc/";
  char fdlnk[512];
  char fdtarget[512];
  ssize_t tgtlength=0;
  
  strcat(filename,pid_s);
  strcat(filename,"/fd");
  pid_dir = opendir(filename);
  if(pid_dir == NULL){
    //printf("could not check file descriptors of pid %s\n",pid_s);
    return;
  }
  //printf("checking %s\n",filename);

  do{
    errno=0;
    dp=readdir(pid_dir);
    if(dp!=NULL){
      if(dp->d_type == DT_LNK){
         //printf("checking fd %s\n",dp->d_name);
         sprintf(fdlnk, "%s/%s", filename, dp->d_name);
         //printf("path: %s\n",fdlnk);
         
         if((tgtlength = readlink(fdlnk,fdtarget,512))==-1){
         //  printf("could not check link target %s\n",fdlnk);
           continue;
         }
         fdtarget[tgtlength]='\0';
         //printf("%s -> %s\n",fdlnk,fdtarget);
         if(strcmp(fdtarget, file)==0){
           printf("Pid %s is using %s\n", pid_s, file);
         }
         
      }
     
    }
  }while(dp!=NULL);
  
  
}

int main(int argc, char** argv){

  DIR *proc;
  struct dirent *dp;
  
  if(argc!=2){
    usage(argv[0]);
  }
  
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
        
        check_pid(dp->d_name, argv[1]);
      }
    }
  } while(dp!=NULL);
  if(errno !=0){
    errExit("readdir");
  }
  
  return 0;
}
void usage(const char *name){
  printf("%s <path>\n", name);
  errno = EINVAL;
  errExit("invalid usage");
}