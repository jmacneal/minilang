#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdlib.h>
#include <stdio.h>
#include "symbol.h"
#include "tree.h"

SymbolTable *stable;

void generatePROG(SymbolTable *s, PROG *prog, char *outputFileName);
void generateDEC(DEC *dec, FILE *outputFile);
void generateSTMT(STMT *stmt, FILE *outputFile);
void generateEXP(EXP *exp, FILE *outputFile);



#endif
