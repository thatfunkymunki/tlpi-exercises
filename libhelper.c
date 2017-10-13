#include "libhelper.h"
#include <string.h>
#include <ctype.h>

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