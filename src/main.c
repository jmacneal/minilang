#include <stdio.h>
#include <string.h>
#include "tree.h"
#include "pretty.h"

void yyparse();
int yylex();

PROG *root = NULL;

int main(int argc, char *argv[])
{
  extern int g_tokens;

  if(strcmp(argv[1], "scan") == 0){
    g_tokens = 0;
    while(yylex()) {}
  }

  else if (strcmp(argv[1], "tokens") == 0){
    g_tokens = 1;
    while(yylex()) {}
  }

  else if (strcmp(argv[1], "parse") == 0){
    g_tokens = 0;
    yyparse();
  }
  else if (strcmp(argv[1], "pretty") == 0){
    g_tokens = 0;
    yyparse();
    prettyPROG(root);
  }

  printf("OK\n");
  return 0;
}
