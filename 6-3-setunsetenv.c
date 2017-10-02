/*Exercise 6-3
 * Implement setenv() and unsetenv() using getenv(), putenv(), and,
 * where necessary, code that directly modifies environ. Your version
 * of unsetenv() should check to see whether there are multiple definitions 
 * of an environment variable, and remove them all (which is what the glibc
 * version of unsetenv() does)
 */


#define _GNU_SOURCE     /* Get various declarations from <stdlib.h> */
#include <stdlib.h>
#include <string.h>
#include "tlpi_hdr.h"


int _setenv(const char *name, const char *value, int overwrite);
int _unsetenv(const char *name);

int _setenv(const char *name, const char *value, int overwrite){
  char *newstr;

  if( (getenv(name) != NULL) && (overwrite == 0) ){
    //already exists and no overwrite
    return 0;
  }
  else{
    unsetenv(name);
    //allocate a new string: length of name + = + length of value + \0 = length of both strings + 2
    newstr = (char *)malloc(strlen(name)+strlen(value)+2);
    sprintf(newstr, "%s=%s", name, value);
    //printf("passing %s to putenv\n", newstr);
  }
  return (putenv(newstr)!=0) ? 1 :0;

}

int _unsetenv(const char *name){
  
  /*iterate through environ
   * if name is found, remove it and shift the remaining ones up
   */
   extern char **environ;

   char **envp;
   size_t len = strlen(name);
   
   
   for(envp = environ; *envp != NULL;){
    // printf("entering loop\n");
    //printf("checking %s\n", *envp);
     if((strncmp(*envp, name, len) == 0) && ((*envp)[len] == '=')){
       //found, remove it by shifting the remainder up
      // printf("%s found\n", name);
       for(char **shift = envp; *shift!=NULL ;shift++){
         *shift = *(shift+1);
       }
       
       
     }
     else{
       envp++;
     }
     
   }
  
  
  return 0;
}

int main(int argc, char **argv){
  setenv("a", "b", 1);
  printf("a = %s\n", getenv("a"));
  putenv("a=c");
  printf("a = %s\n", getenv("a"));
  _setenv("a","d",1);
  printf("a = %s\n", getenv("a"));
  _setenv("b","d",1);
  printf("b = %s\n", getenv("b"));
  _unsetenv("a");
  printf("a = %s\n", (getenv("a")!=NULL) ? getenv("a") : "unset");
  _unsetenv("b");
  printf("b = %s\n", (getenv("b")!=NULL) ? getenv("b") : "unset");

}
