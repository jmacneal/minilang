#include "symbol.h"
#include "tree.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int hash(char *str) {
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

SymbolTable *scopeSymbolTable(SymbolTable *stable) {
        SymbolTable *t = initSymbolTable();
        t->parent = stable;
        return t;
}

int defSymbol(SymbolTable *stable, char *identifier)
{
	int i = hash(identifier);

	for (SYMBOL *s = stable->table[i]; s; s = s->next)
		if (strcmp(s->identifier, identifier) == 0)
			return 1;

	return 0;
}

SYMBOL *putSymbol(SymbolTable *stable, char* identifier, Type type, int lineno) {
        int i = hash(identifier);
        for (SYMBOL *s = stable->table[i]; s; s = s->next) {
                if (strcmp(s->identifier, identifier) == 0) { // Throw Error
                        fprintf (stderr, "Error: (line %d) Illegal redeclaration of symbol '%s'\n",
                                 lineno, identifier);
                        exit(1);
                }
        }
        SYMBOL *s = malloc(sizeof(SYMBOL));
        s->identifier = malloc((strlen(identifier)+1)*sizeof(char));
        strcpy(s->identifier, identifier);
        s->type = type;
        s->next = stable->table[i];
        stable->table[i] = s;
        return s;
}

SYMBOL *getSymbol(SymbolTable *stable, char *identifier) {
        int i = hash(identifier);
        for (SYMBOL *s = stable->table[i]; s; s = s->next) { // Check the current scope
                if (strcmp(s->identifier, identifier) == 0) return s;
        }
// Check for existence of a parent scope
        if (stable->parent == NULL)
                return NULL;
// Check the parent scopes
        return getSymbol(stable->parent, identifier);
}



/*
  Generate a symbol table from an AST

  Parameters:
  | *prog: pointer to PROG structure, root of program's AST

  Return: SymbolTable pointer populated by AST
*/
SymbolTable *genSymbolTable(PROG *prog) {
        SymbolTable *t = initSymbolTable();

        if (prog == NULL)
                return t;

	if (prog->rootDeclaration != NULL)
		genSymbolsDECS(t, prog->rootDeclaration);

	if (prog->rootStatement != NULL)
                genSymbolsSTMTS(t, prog->rootStatement);

        return t;
}

void printSymbol(SYMBOL *symbol){
        switch(symbol->type){
        case t_float:
                printf("| Symbol: %s | Type: float |\n", symbol->identifier);
                break;
        case t_int:
                printf("| Symbol: %s | Type: int |\n", symbol->identifier);
                break;
        case t_string:
                printf("| Symbol: %s | Type: string |\n", symbol->identifier);
                break;
        case t_bool:
                printf("| Symbol: %s | Type: bool |\n", symbol->identifier);
                break;
        }

}
void genSymbolsDECS(SymbolTable *stable, DEC *rootDeclaration) {
        DEC *currentDec = rootDeclaration;
        while (currentDec != NULL){
                genSymbolsEXP(stable, currentDec->rhs);
                SYMBOL *symbol = putSymbol(stable, currentDec->identifier, currentDec->type->val, currentDec->lineno);
                printSymbol(symbol);
                currentDec = currentDec->nextDec;
        }
}

void genSymbolsSTMTS(SymbolTable *stable, STMT *rootStatement) {
        STMT *currentStmt = rootStatement;
        while (currentStmt != NULL) {
                genSymbolsSTMT(stable, currentStmt);
                currentStmt = currentStmt->nextStmt;
        }
}

void genSymbolsSTMT(SymbolTable *stable, STMT *stmt)
{
	SymbolTable *new_st;
	SymbolTable *extra_st;		//this is used for the else block in an if-else stmt

	if (stmt != NULL)
	{
		switch (stmt->kind)
		{
                case k_stmtKindIf:
                        genSymbolsEXP(stable, stmt->val.ifStmt.conditionExp);
                        new_st = scopeSymbolTable(stable);
                        genSymbolsSTMTS(new_st, stmt->val.ifStmt.bodyStmt);
                        break;

                case k_stmtKindIfElse:
                        genSymbolsEXP(stable, stmt->val.ifelseStmt.conditionExp);

                        new_st = scopeSymbolTable(stable);
                        genSymbolsSTMTS(new_st, stmt->val.ifelseStmt.bodyStmt);

                        extra_st = scopeSymbolTable(stable);
                        genSymbolsSTMTS(extra_st, stmt->val.ifelseStmt.elseStmt);
                        break;

                case k_stmtKindWhile:
                        genSymbolsEXP(stable, stmt->val.whileStmt.conditionExp);

                        new_st = scopeSymbolTable(stable);
                        genSymbolsSTMTS(new_st, stmt->val.whileStmt.bodyStmt);
                        break;

                case k_stmtKindRead:
                        if (!getSymbol(stable, stmt->val.readId))
                        {
                                fprintf(stderr, "Error: Var %s has not been declared (line %d)\n",
                                        stmt->val.assignStmt.identifier, stmt->lineno);
                                exit(1);
                        }
                        break;

                case k_stmtKindPrint:
                        genSymbolsEXP(stable, stmt->val.printExp);
                        break;

                case k_stmtKindAssign:
                        genSymbolsEXP(stable, stmt->val.assignStmt.rhs);
                        if (!getSymbol(stable, stmt->val.assignStmt.identifier))
                        {
                                fprintf(stderr, "Error: Var %s has not been declared (line %d) \n",
                                        stmt->val.assignStmt.identifier, stmt->lineno);
                                exit(1);
                        }
                        break;
                }
        }
}


void genSymbolsEXP(SymbolTable *stable, EXP *exp) {
	if (exp != NULL)
	{
		switch (exp->kind)
		{
			case k_expressionKindIdentifier:

				if (getSymbol(stable, exp->val.identifier) == NULL)
				{
					fprintf(stderr, "Error: (line %d) Variable %s has not been declared.\n",
                                                exp->lineno, exp->val.identifier);
					exit(1);
				}
                                break;

			case k_expressionKindAddition:
				genSymbolsEXP(stable, exp->val.binary.lhs);
				genSymbolsEXP(stable, exp->val.binary.rhs);
				break;
			case k_expressionKindSubtraction:
				genSymbolsEXP(stable, exp->val.binary.lhs);
				genSymbolsEXP(stable, exp->val.binary.rhs);
				break;
			case k_expressionKindDivision:
				genSymbolsEXP(stable, exp->val.binary.lhs);
				genSymbolsEXP(stable, exp->val.binary.rhs);
				break;
			case k_expressionKindAnd:
				genSymbolsEXP(stable, exp->val.binary.lhs);
				genSymbolsEXP(stable, exp->val.binary.rhs);
				break;
			case k_expressionKindOr:
				genSymbolsEXP(stable, exp->val.binary.lhs);
				genSymbolsEXP(stable, exp->val.binary.rhs);
				break;
			case k_expressionKindEquals:
				genSymbolsEXP(stable, exp->val.binary.lhs);
				genSymbolsEXP(stable, exp->val.binary.rhs);
				break;
			case k_expressionKindNotEquals:
				genSymbolsEXP(stable, exp->val.binary.lhs);
				genSymbolsEXP(stable, exp->val.binary.rhs);
				break;
			case k_expressionKindUMinus:
				genSymbolsEXP(stable, exp->val.unary.child);
				break;
			case k_expressionKindBang:
				genSymbolsEXP(stable, exp->val.unary.child);
				break;
			//Default case should catch literal constants (int/float/bool/string)
			default:
				break;
		}
	}
}
