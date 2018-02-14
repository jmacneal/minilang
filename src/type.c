#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "type.h"


SymbolTable *stable;

/*
  Run through the AST and confirm that all identifier uses are well-typed
  otherwise emit a helpful error message.
*/
void typeCheckPROG(SymbolTable *global_stable, PROG *prog){
        stable = global_stable;
        if (prog != NULL && prog->rootStatement != NULL)
                typeCheckSTMT(prog->rootStatement);
}

/*
  Check the types for each STMT in the STMT list rooted at *stmt
*/
void typeCheckSTMT(STMT *stmt){
        while (stmt != NULL)
        {
                switch (stmt->kind)
                {
                        //The condition of an if statement must be an integer or a boolean
                case k_stmtKindIf:
                        typeCheckEXP(stmt->val.ifStmt.conditionExp);

                        if (stmt->val.ifStmt.conditionExp->type != t_int ||
                            stmt->val.ifStmt.conditionExp->type != t_bool)
                        {
                                fprintf(stderr, "Error: Type error (line %d): condition must be int or boolean.", stmt->lineno);
                                exit(1);
                        }

                        typeCheckSTMT(stmt->val.ifStmt.bodyStmt);
                        break;

                        //The condition of an ifelse statement must be an integer or a boolean
                case k_stmtKindIfElse:
                        typeCheckEXP(stmt->val.ifelseStmt.conditionExp);

                        if (stmt->val.ifelseStmt.conditionExp->type != t_int ||
                            stmt->val.ifelseStmt.conditionExp->type != t_bool)
                        {
                                fprintf(stderr, "Error: Type error (line %d): condition must be int or boolean.", stmt->lineno);
                                exit(1);
                        }

                        typeCheckSTMT(stmt->val.ifelseStmt.bodyStmt);
                        typeCheckSTMT(stmt->val.ifelseStmt.elseStmt);
                        break;
                        //The condition of an if statement must be an integer or a boolean

                case k_stmtKindWhile:
                        typeCheckEXP(stmt->val.whileStmt.conditionExp);

                        if (stmt->val.whileStmt.conditionExp->type != t_int ||
                            stmt->val.whileStmt.conditionExp->type != t_bool)
                        {
                                fprintf(stderr, "Error: Type error (line %d): condition must be int or boolean.", stmt->lineno);
                                exit(1);
                        }

                        typeCheckSTMT(stmt->val.whileStmt.bodyStmt);
                        break;

                        /* Run-time value, so can't be type-checked */
                case k_stmtKindRead:
                        break;

                case k_stmtKindPrint:
                        typeCheckEXP(stmt->val.printExp);
                        break;

                case k_stmtKindAssign:
                        typeCheckAssignment(stmt);
                        break;
                }
                stmt = stmt->nextStmt;
        }
}

/*
  Type checking for assignment statements is a special case
  both the left side (identifier) and right side expression must be confirmed to have the same type
*/
void typeCheckAssignment(STMT *stmt){
	if(stmt!= NULL){
                EXP *rhs = stmt->val.assignStmt.rhs;
                char *identifier = stmt->val.assignStmt.identifier;
                SYMBOL *symbol = getSymbol(stable, identifier);
		Type type = symbol->type;

		typeCheckEXP(rhs);


                if (type != rhs->type){
                        printf("Poorly typed assignment (line %d). Expected ", stmt->lineno);
                        switch(type){
                        case t_int:
                                printf("t_int.\n");
                                break;
                        case t_float:
                                printf("t_float.\n");
                                break;
                        case t_string:
                                printf("t_string.\n");
                                break;
                        case t_bool:
                                printf("t_bool.\n");
                                break;

                        }
                        exit(1);
                }

                rhs->type = type;
        }
}


/*
  First, checks that the subexpressions in exp are well-typed (no float + string)
  Second, sets exp->type for reference by the code generator
*/
void typeCheckEXP(EXP *exp){
	SYMBOL *sym;
	if (exp != NULL)
	{


		switch (exp->kind)
		{
                case k_expressionKindIdentifier:
                        sym = getSymbol(stable, exp->val.identifier);

                        if (sym == NULL)
                        {
                                fprintf(stderr, "Error: (line %d) Variable %s has not been declared.\n",
                                        exp->lineno, exp->val.identifier);
                                exit(1);
                        }

                        switch (sym->type)
                        {
                        case t_int:
                                exp->type = t_int;
                                break;
                        case t_float:
                                exp->type = t_float;
                                break;
                        case t_string:
                                exp->type = t_string;
                                break;
                        case t_bool:
                                exp->type = t_bool;
                                break;
                        }
                        break;

                case k_expressionKindBoolVal:
                        exp->type = t_bool;
                        break;

                case k_expressionKindStringVal:
                        exp->type = t_string;
                        break;

                case k_expressionKindIntVal:
                        exp->type = t_int;
                        break;

                case k_expressionKindFloatVal:
                        exp->type = t_float;
                        break;


                case k_expressionKindAddition:
                        typeCheckEXP(exp->val.binary.lhs);
                        typeCheckEXP(exp->val.binary.rhs);


                        if (exp->val.binary.lhs->type == t_int)
                        {
                                if (exp->val.binary.rhs->type == t_int)
                                        exp->type = t_int;
                                else if (exp->val.binary.rhs->type == t_float)
                                        exp->type = t_float;
                                else
                                {
                                        fprintf(stderr, "Error: Can only add int to int or float.\n");
                                        exit(1);
                                }
                        }

                        else if (exp->val.binary.lhs->type == t_float)
                        {
                                if (exp->val.binary.rhs->type == t_int)
                                        exp->type = t_float;
                                else if (exp->val.binary.rhs->type == t_float)
                                        exp->type = t_float;
                                else
                                {
                                        fprintf(stderr, "Error: Can only add float to int or float.\n");
                                        exit(1);
                                }
                        }

                        else if (exp->val.binary.lhs->type == t_string)
                        {
                                if (exp->val.binary.rhs->type == t_string)
                                        exp->type = t_string;
                                else
                                {
                                        fprintf(stderr, "Error: Can only add string to string.\n");
                                        exit(1);
                                }
                        }
                        break;

                case k_expressionKindSubtraction:
                        typeCheckEXP(exp->val.binary.lhs);
                        typeCheckEXP(exp->val.binary.rhs);



                        if (exp->val.binary.lhs->type == t_int)
                        {
                                if (exp->val.binary.rhs->type == t_int)
                                        exp->type = t_int;
                                else if (exp->val.binary.rhs->type == t_float)
                                        exp->type = t_float;
                                else
                                {
                                        fprintf(stderr, "Error: Subtracting with non-int/non-float value.\n");
                                        exit(1);
                                }
                        }

                        else if (exp->val.binary.lhs->type == t_float)
                        {
                                if (exp->val.binary.rhs->type == t_int)
                                        exp->type = t_float;
                                else if (exp->val.binary.rhs->type == t_float)
                                        exp->type = t_float;
                                else
                                {
                                        fprintf(stderr, "Error: Subtracting with non-int/non-float value.\n");
                                        exit(1);
                                }
                        }
                        break;

                case k_expressionKindMultiplication:
                        typeCheckEXP(exp->val.binary.lhs);
                        typeCheckEXP(exp->val.binary.rhs);



                        if (exp->val.binary.lhs->type == t_int)
                        {
                                if (exp->val.binary.rhs->type == t_int)
                                        exp->type = t_int;
                                else if (exp->val.binary.rhs->type == t_float)
                                        exp->type = t_float;
                                else if (exp->val.binary.rhs->type == t_string)
                                {
                                        exp->type = t_string;

                                }
                                else
                                {
                                        fprintf(stderr, "Error: Multiplying with non-int/non-float value.\n");
                                        exit(1);
                                }
                        }

                        else if (exp->val.binary.lhs->type == t_float)
                        {
                                if (exp->val.binary.rhs->type == t_int)
                                        exp->type = t_float;
                                else if (exp->val.binary.rhs->type == t_float)
                                        exp->type = t_float;
                                else
                                {
                                        fprintf(stderr, "Error: Multiplying with non-int/non-float value.\n");
                                        exit(1);
                                }
                        }

                        else if (exp->val.binary.lhs->type == t_string)
                        {
                                if (exp->val.binary.rhs->type == t_int)
                                {
                                        exp->type = t_string;

                                }
                                else
                                {
                                        fprintf(stderr, "Error: Multiplying string with non-int value.\n");
                                        exit(1);
                                }
                        }
                        break;

                        break;

                case k_expressionKindDivision:
                        typeCheckEXP(exp->val.binary.lhs);
                        typeCheckEXP(exp->val.binary.rhs);
                        break;

                case k_expressionKindAnd:
                        typeCheckEXP(exp->val.binary.lhs);
                        typeCheckEXP(exp->val.binary.rhs);
                        break;

                case k_expressionKindOr:
                        typeCheckEXP(exp->val.binary.lhs);
                        typeCheckEXP(exp->val.binary.rhs);
                        exp->type = t_bool;
                        break;

                case k_expressionKindEquals:
                        exp->type = t_bool;
                        typeCheckEXP(exp->val.binary.lhs);
                        typeCheckEXP(exp->val.binary.rhs);
                        break;

                case k_expressionKindNotEquals:
                        exp->type = t_bool;
                        typeCheckEXP(exp->val.binary.lhs);
                        typeCheckEXP(exp->val.binary.rhs);
                        break;

                case k_expressionKindBang:
                        typeCheckEXP(exp->val.unary.child);
                        break;

                case k_expressionKindUMinus:
                        typeCheckEXP(exp->val.unary.child);
                        break;

		}
	}
}
