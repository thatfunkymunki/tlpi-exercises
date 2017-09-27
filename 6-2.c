/* Exercise 6-2
 * Write a program to see what happens if we try to longjmp() into a function
 * that has already returned.
 */
#include <setjmp.h>
#include "tlpi_hdr.h"

jmp_buf env;

void f1(){
  switch(setjmp(env)){
    case 0:
      printf("called setjmp in f1()\n");
      break;
    case 1:
      printf("jumped back into f1()\n");
      break;
  }
  return;
}

void f2(){
  printf("jumping back into f1() from f2()\n");
  longjmp(env, 1);
}

int main(int argc, char *argv[]){
  printf("calling f1() from main()\n");
  f1();
  printf("calling f2() from main()\n");
  f2();
}
