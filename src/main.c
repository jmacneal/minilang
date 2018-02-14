#include <stdio.h>
#include <string.h>
#include "tree.h"
#include "pretty.h"
#include "symbol.h"
#include "type.h"
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
                printf("OK\n");
        }

        else if (strcmp(argv[1], "typecheck") == 0){
                g_tokens = 0;
                yyparse();
                stable = genSymbolTable(root);
                typeCheckPROG(stable, root);
                printf("OK\n");
        }

        else if (strcmp(argv[1], "codegen") == 0){
                g_tokens = 0;
                yyparse();
                stable = genSymbolTable(root);
                /* typeCheckPROG(stable, root); */
                char* outputFileName = malloc((strlen(argv[2])+1)*sizeof(char));
                for (int i=0; (argv[2][i] != '.'); i++)
                        outputFileName[i] = argv[2][i];
                strcat(outputFileName, ".c");
                generatePROG(stable, root, outputFileName);
                printf("OK\n");
        }

        return 0;
}
