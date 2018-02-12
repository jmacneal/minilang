#include "symbol.h"

int Hash(char *str) {
        unsigned int hash = 0;
        while (*str) hash = (hash << 1) + *str++;
        return hash % HashSize;
}

SymbolTable *initSymbolTable() {
        SymbolTable *t = malloc(sizeof(SymbolTable));
        for (int i = 0; i < HashSize; i++) {
                t->table[i] = NULL;
        }
        t->parent = NULL;
        return t;
}


SymbolTable *scopeSymbolTable(SymbolTable *s) {
        SymbolTable *t = initSymbolTable();
        t->parent = s;
        return t;
}

SYMBOL *putSymbol(SymbolTable *t, char* name, SymbolKind kind) {
        int i = Hash(name);
        for (SYMBOL *s = t->table[i]; s; s = s->next) {
                if (strcmp(s->name, name) == 0) // throw an error
                        }
        SYMBOL *s = malloc(sizeof(SYMBOL));
        s->name = name;
        s->kind = kind;
        s->next = t->table[i];
        t->table[i] = s;
        return s;
}

SYMBOL *getSymbol(SymbolTable *t, char *name) {
        int i = Hash(name);
// Check the current scope
        for (SYMBOL *s = t->table[i]; s; s = s->next) {
                if (strcmp(s->name, name) == 0) return s;
        }
// Check for existence of a parent scope
        if (t->parent == NULL)
                return NULL;
// Check the parent scopes
        return getSymbol(t->parent, name);
}
