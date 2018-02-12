// Base tree structure for AST (base copied from tinylang)

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tree.h"

extern int yylineno;

/* PROG constructor and DEC/STMT insert functions */
PROG *makePROG(DEC *rootDeclaration, STMT *rootStatement)
{
        PROG *p = malloc(sizeof(PROG));
        p->rootDeclaration = rootDeclaration;
        p->rootStatement = rootStatement;
        return p;
}

/* Add a new declaration to the end of prog's DEC linked list
 dec must already be malloc'd*/
DEC *insertDEC(DEC *decList, DEC *dec)
{
        if(decList == NULL)
        {
                decList = dec;
        }
        else{
                DEC *currentDec = decList;

                while(currentDec->nextDec != NULL)
                        currentDec = currentDec->nextDec;

                currentDec->nextDec = dec;
        }

        return decList;
}

/* Add a new statement to the end of prog's STMT linked list
 stmt must already be malloc'd*/
STMT *insertSTMT(STMT *stmtList, STMT *stmt)
{
        if(stmtList == NULL)
        {
                stmtList = stmt;
        }
        else{
                STMT *currentStmt = stmtList;

                while(currentStmt->nextStmt != NULL)
                        currentStmt = currentStmt->nextStmt;

                currentStmt->nextStmt = stmt;
        }

        return stmtList;
}


/* DEC constructor*/
DEC *makeDEC(TYPE *type, char *identifier, EXP *rhs)
{
        DEC *d = malloc(sizeof(DEC));
        d->lineno = yylineno;
        d->type = type;
        d->identifier = malloc((strlen(identifier)+1)*sizeof(char));
        strcpy(d->identifier, identifier);
        d->rhs = rhs;
        d->nextDec = NULL;
        return d;
}


/* STMT constructors */
STMT *makeSTMT_assign(const char *identifier, EXP *rhs)
{

        STMT *s = malloc(sizeof(STMT));
        s->lineno = yylineno;
        s->kind = k_stmtKindAssign;
        s->val.assignStmt.identifier = malloc(strlen(identifier)+1);
        strcpy(s->val.assignStmt.identifier, identifier);
        /* s->val.assignStmt.identifier = identifier; */
        s->val.assignStmt.rhs = rhs;
        s->nextStmt = NULL;


        return s;
}

STMT *makeSTMT_if(EXP *conditionExp, STMT *bodyStmt)
{
        STMT *s = malloc(sizeof(STMT));
        s->lineno = yylineno;
        s->kind = k_stmtKindIf;
        s->val.ifStmt.conditionExp = conditionExp;
        s->val.ifStmt.bodyStmt = bodyStmt;
        s->nextStmt = NULL;
        return s;
}

STMT *makeSTMT_ifelse(EXP *conditionExp, STMT *bodyStmt, STMT *elseStmt)
{
        STMT *s = malloc(sizeof(STMT));
        s->lineno = yylineno;
        s->kind = k_stmtKindIfElse;
        s->val.ifelseStmt.conditionExp = conditionExp;
        s->val.ifelseStmt.bodyStmt = bodyStmt;
        s->val.ifelseStmt.elseStmt = elseStmt;
        s->nextStmt = NULL;
        return s;
}

STMT *makeSTMT_while(EXP *conditionExp, STMT *bodyStmt)
{
        STMT *s = malloc(sizeof(STMT));
        s->lineno = yylineno;
        s->kind = k_stmtKindWhile;
        s->val.whileStmt.conditionExp = conditionExp;
        s->val.whileStmt.bodyStmt = bodyStmt;
        s->nextStmt = NULL;
        return s;
}

STMT *makeSTMT_print(EXP *printExp)
{
        STMT *s = malloc(sizeof(STMT));
        s->lineno = yylineno;
        s->kind = k_stmtKindPrint;
        s->val.printExp = printExp;
        s->nextStmt = NULL;
        return s;
}

STMT *makeSTMT_read(char *readId)
{
        STMT *s = malloc(sizeof(STMT));
        s->lineno = yylineno;
        s->kind = k_stmtKindRead;
        s->val.readId = malloc((strlen(readId)+1)*sizeof(char));
        strcpy(s->val.readId, readId);
        s->nextStmt = NULL;
        return s;
}


/* EXP constructors */
EXP *makeEXP_identifier(const char *identifier)
{
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionKindIdentifier;
        e->val.identifier = malloc((strlen(identifier)+1)*sizeof(char));
	strcpy(e->val.identifier, identifier);
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

EXP *makeEXP_stringval(const char *string){
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expressionKindStringVal;
        e->val.stringval = malloc((strlen(string)+1)*sizeof(char));
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

TYPE *makeTYPE_int();TYPE *makeTYPE_int()
{
        TYPE *t = malloc(sizeof(TYPE));
        t->val = t_int;
        return t;
}

TYPE *makeTYPE_float()
{
        TYPE *t = malloc(sizeof(TYPE));
        t->val = t_float;
        return t;
}

TYPE *makeTYPE_string()
{
        TYPE *t = malloc(sizeof(TYPE));
        t->val = t_string;
        return t;
}

TYPE *makeTYPE_bool()
{
        TYPE *t = malloc(sizeof(TYPE));
        t->val = t_bool;
        return t;
}
