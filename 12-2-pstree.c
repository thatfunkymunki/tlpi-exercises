/*Exercise 12-2
 * Write a program that draws a tree showing the hierarchical parent-child
 * relationships of all processes on the system, going all the way back to init. For each
 * process, the program should display the process ID and the command being
 * executed. The output of the program should be similar to that produced by pstree(1),
 * although it does need not to be as sophisticated. The parent of each process on the
 * system can be found by inspecting the PPid: line of all of the /proc/PID/status files
 * on the system. Be careful to handle the possibility that a process’s parent (and thus
 * its /proc/PID directory) disappears during the scan of all /proc/PID directories.
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
  int childcount;
  struct process *parent;
  struct process *children[CHILDREN_MAX];
} process_node; 

int string_is_number(const char *string);
char *trimstring(char *str);
process_node *create_node(const char *pid);
void generate_tree(void);
void print_tree(process_node *tree);
void print_list(void);
void find_children(process_node *proc);
void add_child(process_node *parent, process_node *child);


process_node *node_list[PROCESS_MAX];
int nodecount = 0;

process_node *node_tree;
int treenodecount = 0;


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
void print_list(){
  for(int i=0;i<nodecount;i++){
    printf("pid: %d ppid: %d\n",node_list[i]->pid,node_list[i]->ppid);
  }
}
void generate_tree(){
  //first find tree root aka init aka pid 1
  for(int i=0;i<nodecount;i++){
    if(node_list[i]->pid==1){
      node_tree=node_list[i];
      treenodecount++;
      break;
    }
  }
  //find children of init first
  find_children(node_tree);
  
  //iterate through the rest of the tree
 
}
void find_children(process_node *proc){
  for(int i=0;i<nodecount;i++){
    if(node_list[i]->ppid == proc->pid){
      add_child(proc,node_list[i]);
      treenodecount++;
      find_children(node_list[i]);
    }
  }
}
void add_child(process_node *parent, process_node *child){
  parent->children[parent->childcount]=child;
  parent->childcount++;
  child->parent=parent;
}
void print_tree(process_node *node){
  if(node==NULL){
    printf("\n");
    return;
  }
  printf("--");
  printf("%d\n",node->pid);
  for(int i=0;i<node->childcount;i++){
    
    print_tree(node->children[i]);
  }
  printf("--");
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
    newnode->childcount=0;
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
  
  
  
  //turn list into tree
  
  generate_tree();
  
  //print out tree
  print_list();
  
  print_tree(node_tree);
  
  
  
  
  if(closedir(proc) == -1){
    errExit("closedir");
  }
  
  
  return 0;
}
