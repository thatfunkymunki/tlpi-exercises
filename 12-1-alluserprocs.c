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
#include <ctype.h>
#include <string.h>

void usage(const char *name);
int string_is_number(const char *string);
int pidstatus(const char *path, uid_t uid);
char *trimstring(char *str);

int string_is_number(const char *string){
  while(*string){
    if(isdigit(*string++) == 0) return 0;
  }
  return 1;
}
char *trimstring(char *str){ //based on https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way
  char *end;
  //get rid of leading whitespace
  while(isspace(*str)){ str++; }
  if(*str=='\0'){ //all spaces
    return str;
  }
  end = str + strlen(str) -1;
  while (end > str && isspace(*end)){ end--; }
  *(end+1)='\0';
  return str;
}
int pidstatus(const char *path, uid_t uid){
  char filename[256] = "/proc/";
  char inbuffer[256];
  char name[256] = "\0";
  char uid_s[256];
  sprintf(uid_s, "%d", uid);
  
  
  FILE *statusfile;
  strcat(filename,path);
  strcat(filename,"/status");
  
  if ( (statusfile = fopen(filename, "r")) == NULL){
    return 1;
  }
  
  //printf("%s\n",path);
  
  int found = 0;
  while(fgets(inbuffer,sizeof(inbuffer),statusfile) != NULL){
    char *field = trimstring(strtok(inbuffer, ":"));
    char *val = strtok(trimstring(strtok(NULL, ":")), "\t"); //coerce the value into a single token (don't need all the different UIDs of a process for example)
    
    if(strcmp(field, "Uid") == 0){
     // printf("%s : %s\n", field, val);
      if(strcmp(val, uid_s) == 0){
        found = 1;  
      }
    }
    if(strcmp(field, "Name") == 0){
    //printf("%s : %s\n", field, val);
      strcpy(name, val);
    }
  }
  if(found == 1){
    printf("UID: %s PID: %s Name: %s\n",uid_s, path, name);
  }
  
  
  
  
  //printf("%s\n",filename);
  return 0;
}

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
  
  //printf("user %s id %d\n", argv[1],uid);
  
  proc = opendir("/proc/");
  if(proc == NULL){
    errExit("failed opening /proc/");
  }
  do{
    errno=0;
    dp = readdir(proc);
    if(dp!=NULL){
      
      if(dp->d_type == DT_DIR || dp->d_type == DT_LNK){ //walk all dirs and symlinks
        if( string_is_number(dp->d_name) == 0) { //only look through PID dirs
          continue;
        }
        
        //printf("%s\n", dp->d_name);
        pidstatus(dp->d_name, uid);
      }
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