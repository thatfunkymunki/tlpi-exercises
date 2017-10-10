/*Exercise 12-1
 * Write a program that lists the process ID and command name for all processes
 * being run by the user named in the program’s command-line argument. (You may
 * find the userIdFromName() function from Listing 8-1, on page 159, useful.) This can
 * be done by inspecting the Name: and Uid: lines of all of the /proc/PID/status files on
 * the system. Walking through all of the /proc/PID directories on the system requires the
 * use of readdir(3), which is described in Section 18.8. Make sure your program
 * correctly handles the possibility that a /proc/PID directory disappears between the
 * time that the program determines that the directory exists and the time that it tries
 * to open the corresponding /proc/PID/status file.
 */
 
#include "tlpi_hdr.h"
#include "ugid_functions.h"   /* userNameFromId() & groupNameFromId() */
#include <dirent.h>

void usage(const char *name);

int main(int argc, char** argv){
  
  DIR *proc;
  struct dirent *dp;
  
  uid_t uid;
  if(argc!=2){
    usage(argv[0]);
  }
  
  if( (uid = userIdFromName(argv[1])) == -1){
    errExit("invalid user");
  }
  
  printf("user %s id %d\n", argv[1],uid);
  
  proc = opendir("/proc/");
  if(proc == NULL){
    errExit("failed opening /proc/");
  }
  do{
    errno=0;
    dp = readdir(proc);
    if(dp!=NULL){
      
      if(dp->d_type == DT_DIR || dp->d_type == DT_LNK){
    
      }
      if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0){
        continue;
      }
      printf("%s\n", dp->d_name);
      
      
    }
    
    
    
    
    
    
    
    
    
  } while(dp!=NULL);
  if(errno !=0){
    errExit("readdir");
  }
  if(closedir(proc) == -1){
    errExit("closedir");
  }
  
  
  return 0;
}
void usage(const char *name){
  printf("%s <user>\n", name);
  errno = EINVAL;
  errExit("invalid usage");
}