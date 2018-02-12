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
    /* printf("Scanning\n"); */
    g_tokens = 0;
    while(yylex()) {}
  }

  else if (strcmp(argv[1], "tokens") == 0){
    /* printf("Scanning with tokens\n"); */
    g_tokens = 1;
    while(yylex()) {}
  }

  else if (strcmp(argv[1], "parse") == 0){
    /* printf("Parsing\n"); */
    g_tokens = 0;
    /* root = makePROG(); */
    yyparse();
    prettyPROG(root);
    /* int result = evalEXP(root); */
    /* printf("\nEvaluation: "); */

    /* printf(" = %d\n", result); */
  }

  else{

  }
  printf("OK\n");
  return 0;
}
