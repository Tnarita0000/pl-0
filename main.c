#include <stdio.h>
#include "compile.c"
#ifndef GET_SOURCE
#define GET_SOURCE
#include "getSource.c"
#endif

int main() {
  char fileName[30];
  printf("enter source file name\n");
  scanf("%s", fileName);

  if (!openSource(fileName))
    return 0;

  if (compile())
    execute();

  // closeSource();
  return 1;
}
