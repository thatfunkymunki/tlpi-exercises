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
#define PROCESS_MAX 1024
#define CHILDREN_MAX 256

typedef struct process{
  pid_t pid;
  pid_t ppid;
  struct process *parent;
  struct process *children[CHILDREN_MAX];
} process_node; 

int string_is_number(const char *string);
char *trimstring(char *str);
process_node *create_node(const char *pid);

process_node *node_list[PROCESS_MAX];
int nodecount = 0;

process_node *node_tree;


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
process_node *create_node(const char *pid_s){
  char filename[256] = "/proc/";
  char inbuffer[256];
  char ppid_s[256] = "\0";
  process_node *newnode = NULL;
  
  FILE *statusfile;
  strcat(filename,pid_s);
  strcat(filename,"/status");
  
  if ( (statusfile = fopen(filename, "r")) == NULL){
    return NULL;
  }
  
  //printf("%s\n",path);
  
  while(fgets(inbuffer,sizeof(inbuffer),statusfile) != NULL){
    char *field = trimstring(strtok(inbuffer, ":"));
    char *val = strtok(trimstring(strtok(NULL, ":")), "\t"); //coerce the value into a single token (don't need all the different UIDs of a process for example)
    
    if(strcmp(field, "PPid") == 0){
     // printf("%s : %s\n", field, val);
      strcpy(ppid_s, val);
      break;
    }
  }
  fclose(statusfile);
  //printf("Pid: %s PPid: %s\n", pid_s, ppid_s);
  if(strcmp(ppid_s,"\0")!=0){
    newnode= malloc(sizeof(process_node));
    newnode->pid=atoi(pid_s);
    newnode->ppid=atoi(ppid_s);
    newnode->parent=NULL;
    for(int i=0;i<CHILDREN_MAX;i++){
      newnode->children[i]=NULL;
    }
    return newnode;
  }
  return NULL;
}

int main(int argc, char** argv){
  
  DIR *proc;
  struct dirent *dp;
  
 
  
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
        process_node *newnode=create_node(dp->d_name);
        if(newnode!=NULL){
          node_list[nodecount]=newnode;
          nodecount++;
        }
      }
    }
  } while(dp!=NULL && nodecount < PROCESS_MAX);
  if(errno !=0){
    errExit("readdir");
  }
  
  
  for(int i=0;i<nodecount;i++){
    printf("pid: %d ppid: %d\n",node_list[i]->pid,node_list[i]->ppid);
  }
  //turn list into tree
  
  //print out tree
  
  
  
  
  if(closedir(proc) == -1){
    errExit("closedir");
  }
  
  
  return 0;
}
