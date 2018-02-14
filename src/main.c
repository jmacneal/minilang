#include <stdio.h>
#include <string.h>
#include "tree.h"
#include "pretty.h"
#include "symbol.h"
#include "codegen.h"

void yyparse();
int yylex();

PROG *root = NULL;
SymbolTable *stable;

int main(int argc, char *argv[])
{
        extern int g_tokens;

        if(strcmp(argv[1], "scan") == 0){
                g_tokens = 0;
                while(yylex()) {}
                printf("OK\n");
        }

        else if (strcmp(argv[1], "tokens") == 0){
                g_tokens = 1;
                while(yylex()) {}
                printf("OK\n");
        }

        else if (strcmp(argv[1], "parse") == 0){
                g_tokens = 0;
                yyparse();
                printf("OK\n");
        }

        else if (strcmp(argv[1], "pretty") == 0){
                g_tokens = 0;
                yyparse();
                prettyPROG(root);
        }

        else if (strcmp(argv[1], "symbol") == 0){
                g_tokens = 0;
                yyparse();
                stable = genSymbolTable(root);
        }

        else if (strcmp(argv[1], "typecheck") == 0){
                g_tokens = 0;
                yyparse();
                prettyPROG(root);
        }

        else if (strcmp(argv[1], "codegen") == 0){
                g_tokens = 0;
                yyparse();
                stable = genSymbolTable(root);
                generatePROG(stable, root, "mini_generated_c.c");
        }

        return 0;
}
