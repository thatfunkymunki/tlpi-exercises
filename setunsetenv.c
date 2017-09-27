/*Exercise 6-3
 * Implement setenv() and unsetenv() using getenv(), putenv(), and,
 * where necessary, code that directly modifies environ. Your version
 * of unsetenv() should check to see whether there are multiple definitions 
 * of an environment variable, and remove them all (which is what the glibc
 * version of unsetenv() does)
 */


#define _GNU_SOURCE     /* Get various declarations from <stdlib.h> */
#include <stdlib.h>
#include "tlpi_hdr.h"

extern char **environ;

int _setenv(const char *name, const char *value, int overwrite);
int _unsetenv(const char *name);

int _setenv(const char *name, const char *value, int overwrite){

  if( (getenv(name) != NULL) && (overwrite == 0) ){
    //already exists and no overwrite
    return 0;
  }
  else{
    //allocate a new string: length of name + = + length of value + \0
    char *newstr = (char *)malloc(strlen(name)+strlen(value)+2);
    sprintf(newstr, "%s=%s", name, value);
    printf("passing %s to putenv\n", newstr);
    putenv(newstr);
    
  }
  return 0;

}

int _unsetenv(const char *name){
  
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



}
