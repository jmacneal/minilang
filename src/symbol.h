#ifndef SYMBOL_H
#define SYMBOL_H

#include "tree.h"

#define HashSize 317

typedef struct SYMBOL {
        char *identifier;
        Type type;
        struct SYMBOL *next;
} SYMBOL;

typedef struct SymbolTable {
        SYMBOL *table[HashSize];
        struct SymbolTable *parent;
} SymbolTable;


int hash(char *str);
SymbolTable *initSymbolTable();
SymbolTable *scopeSymbolTable(SymbolTable *stable);
SYMBOL *putSymbol(SymbolTable *stable, char* identifier, Type type, int lineno);
SYMBOL *getSymbol(SymbolTable *stable, char *identifier);
int defSymbol(SymbolTable *stable, char *identifier);

void printSymbol(SYMBOL *symbol);

SymbolTable *genSymbolTable(PROG *prog);
void genSymbolsDECS(SymbolTable *stable, DEC *rootDeclaration);
void genSymbolsSTMTS(SymbolTable *stable, STMT *rootStatement);
void genSymbolsSTMT (SymbolTable *stable, STMT *stmt);
void genSymbolsEXP(SymbolTable *stable, EXP *exp);



#endif
