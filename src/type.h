#ifndef TYPE_H
#define TYPE_H

#include "tree.h"
#include "symbol.h"


void typeCheckPROG(SymbolTable *global_stable, PROG *prog);
void typeCheckSTMT(STMT *stmt);
void typeCheckAssignment(STMT *stmt);
void typeCheckEXP(EXP *exp);


#endif
