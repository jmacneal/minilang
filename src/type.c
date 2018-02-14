#include "type.h"


/*
   Run through the AST and confirm that all identifier uses are well-typed
   otherwise emit a helpful error message.
 */
void typeCheckPROG(PROG *prog){
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
                        typeCheckEXP(stmt->val.ifS.condition);

                        if (stmt->val.ifS.condition->type.type != tk_Int ||
                            stmt->val.ifS.condition->type.type != tk_Boolean)
                        {
                                fprintf(stderr, "Error: Type check exception at line %d: the condition of an if statement must be of type int or boolean.", stmt->lineno);
                                exit(1);
                        }

                        typeCheckSTMT(stmt->val.ifS.body);
                        break;

                        //The condition of an ifelse statement must be an integer or a boolean
                case k_stmtKindIfElse:
                        typeCheckEXP(stmt->val.ifelseS.condition);

                        if (stmt->val.ifelseS.condition->type.type != tk_Int ||
                            stmt->val.ifelseS.condition->type.type != tk_Boolean)
                        {
                                fprintf(stderr, "Error: Type check exception at line %d: the condition of an if-else statement must be of type int or boolean.", stmt->lineno);
                                exit(1);
                        }

                        typeCheckSTMT(stmt->val.ifelseS.thenpart);
                        typeCheckSTMT(stmt->val.ifelseS.elsepart);
                        break;
                        //The condition of an if statement must be an integer or a boolean

                case k_stmtKindWhile:
                        typeCheckEXP(stmt->val.whileS.condition);

                        if (stmt->val.whileS.condition->type.type != tk_Int ||
                            stmt->val.whileS.condition->type.type != tk_Boolean)
                        {
                                fprintf(stderr, "Error: Type check exception at line %d: the condition of a while statement must be of type int or boolean.", stmt->lineno);
                                exit(1);
                        }

                        typeCheckSTMT(stmt->val.whileS.body);
                        break;

                        /* Run-time value, so can't be type-checked */
                case k_stmtKindRead:
                        break;

                case k_stmtKindPrint:
                        typeCheckEXP(stmt->val.printS);
                        break;

                case k_stmtKindAssign:
                        typeCheckAssignment(s);
                        break;
                }
                stmt = stmt->next;
        }
}

/*
   Type checking for assignment statements is a special case
   both the left side (identifier) and right side expression must be confirmed to have the same type
 */
void typeCheckAssignment(STMT *stmt){

}
void typeCheckEXP(EXP *exp){

}
