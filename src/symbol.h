#ifndef SYMBOL_H
#define SYMBOL_H

#include "tree.h"

#define HashSize 317

typedef struct SYMBOL {
        char *identifier;
        EXP *val;
        struct SYMBOL *next;
} SYMBOL;

typedef struct SymbolTable {
        SYMBOL *table[HashSize];
        struct SymbolTable *parent;
} SymbolTable;


int Hash(char *str);
SymbolTable *initSymbolTable();
SymbolTable *scopeSymbolTable(SymbolTable *s);
SYMBOL *putSymbol(SymbolTable *t, char* name, TYPE *type);
SYMBOL *getSymbol(SymbolTable *t, char *name);

#endif
