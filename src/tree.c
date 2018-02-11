// Base tree structure for AST (base copied from tinylang)

#include <stdlib.h>
#include <string.h>
#include "tree.h"

extern int yylineno;

EXP *makeEXP_identifier(char *identifier)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionKindIdentifier;
	e->val.identifier = identifier;
	return e;
}

EXP *makeEXP_intval(int intval)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionKindIntVal;
	e->val.intval = intval;
	return e;
}

EXP *makeEXP_stringval(char *string){
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionKindStringVal;
	strcpy(e->val.stringval, string);
	return e;
}


EXP *makeEXP_floatval(float floatval){
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionKindFloatVal;
        e->val.floatval = floatval;
	return e;
}

EXP *makeEXP_boolval(int boolval){
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionKindBoolVal;
        e->val.boolval = boolval;
	return e;
}

EXP *makeEXP_unary(ExpressionKind op, EXP *unary)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = op;
	e->val.unary.child = unary;
	return e;
}

EXP * makeEXP_binary(ExpressionKind op, EXP *lhs, EXP *rhs)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = op;
	e->val.binary.lhs = lhs;
	e->val.binary.rhs = rhs;
	return e;
}
