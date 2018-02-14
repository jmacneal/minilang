#include "codegen.h"

int indentation;

void generatePROG(SymbolTable *s, PROG *prog, char *outputFileName){
        indentation = 1;
        stable = s;

        if (prog == NULL)
                return;

        FILE *outputFile = fopen(outputFileName, "w+");

        /* Write generic include statements to file */
	fprintf(outputFile, "#include <stdio.h>\n");
	fprintf(outputFile, "#include <stdlib.h>\n");
	fprintf(outputFile, "#include <string.h>\n");
	fprintf(outputFile, "#include <stdbool.h>\n");

        /* Write opening of main function */
        fprintf(outputFile, "int main(){\n");

        if (prog->rootDeclaration != NULL){
                generateDEC(prog->rootDeclaration, outputFile);
        }

        if (prog->rootStatement != NULL){
                generateSTMT(prog->rootStatement, outputFile);
        }

        /* Return 0 (minilang doesn't support return values), and close the main() function */
	fprintf(outputFile,"\treturn 0;\n");
	fprintf(outputFile,"}\n");

        fclose(outputFile);
}

void generateDEC(DEC *dec, FILE *outputFile){
        for(int i=0; i<indentation; i++) fprintf(outputFile, "\t");

	switch (dec->type->val)
	{
        case t_int:
                fprintf(outputFile, "int %s = ", dec->identifier);
                generateEXP(dec->rhs, outputFile);
                fprintf(outputFile, ";\n");
                break;
        case t_float:
                fprintf(outputFile, "float %s = ", dec->identifier);
                generateEXP(dec->rhs, outputFile);
                fprintf(outputFile, ";\n");
                break;
        case t_string:
                fprintf(outputFile, "char* %s = ", dec->identifier);
                generateEXP(dec->rhs, outputFile);
                fprintf(outputFile, ";\n");
                break;
        case t_bool:
                fprintf(outputFile, "bool %s = ", dec->identifier);
                generateEXP(dec->rhs, outputFile);
                fprintf(outputFile, ";\n");
                break;
	}

	if (dec->nextDec != NULL)
	{
		generateDEC(dec->nextDec, outputFile);
	}
}

void generateSTMT(STMT *stmt, FILE *outputFile){
        for(int i=0; i<indentation; i++) fprintf(outputFile, "\t");

	switch (stmt->kind)
	{
        case k_stmtKindIf:
                fprintf(outputFile, "if(");
                generateEXP(stmt->val.ifStmt.conditionExp, outputFile);
                fprintf(outputFile, ") {\n");
                indentation++;
                generateSTMT(stmt->val.ifStmt.bodyStmt, outputFile);
                indentation--;
                for(int i=0; i<indentation; i++) fprintf(outputFile, "\t");
                fprintf(outputFile, "}\n");
                break;
        case k_stmtKindIfElse:
                fprintf(outputFile, "if(");
                generateEXP(stmt->val.ifelseStmt.conditionExp, outputFile);
                fprintf(outputFile, ") {\n");
                indentation++;
                generateSTMT(stmt->val.ifelseStmt.bodyStmt, outputFile);
                indentation--;
                fprintf(outputFile, "\n");
                for(int i=0; i<indentation; i++) fprintf(outputFile, "\t");
                fprintf(outputFile, "}\n");
                for(int i=0; i<indentation; i++) fprintf(outputFile, "\t");
                fprintf(outputFile, "else {\n");
                indentation++;
                generateSTMT(stmt->val.ifelseStmt.elseStmt, outputFile);
                indentation--;
                fprintf(outputFile, "\n");
                for(int i=0; i<indentation; i++) fprintf(outputFile, "\t");
                fprintf(outputFile, "}\n");
                break;
        case k_stmtKindWhile:
                fprintf(outputFile, "while(");
                generateEXP(stmt->val.whileStmt.conditionExp, outputFile);
                fprintf(outputFile, ") {\n");
                indentation++;
                generateSTMT(stmt->val.whileStmt.bodyStmt, outputFile);
                indentation--;
                fprintf(outputFile, "\n");
                for(int i=0; i<indentation; i++) fprintf(outputFile, "\t");
                fprintf(outputFile, "}\n");
                break;
        case k_stmtKindRead:
                switch(getSymbol(stable, stmt->val.readId)->type){
                case t_int:
                        fprintf(outputFile, "scanf(\"%%d\", &%s);\n", stmt->val.readId);
                        break;
                case t_float:
                        fprintf(outputFile, "scanf(\"%%f\", &%s);\n", stmt->val.readId);
                        break;
                case t_string:
                        fprintf(outputFile, "scanf(\"%%s\", &%s);\n", stmt->val.readId);
                        break;
                case t_bool:
                        fprintf(outputFile, "scanf(\"%%d\", &%s);\n", stmt->val.readId);
                        break;
                }
                /* fprintf(outputFile, "read %s;\n", stmt->val.readId); */
                break;
        case k_stmtKindPrint:
                printf("expression: %d - type: %d\n", stmt->lineno, stmt->val.printExp->type);
                switch(stmt->val.printExp->type){
                case t_int:
                        fprintf(outputFile, "printf(\"%%d\", ");
                        generateEXP(stmt->val.printExp, outputFile);
                        fprintf(outputFile, ");\n");
                        break;
                case t_float:
                        fprintf(outputFile, "printf(\"%%f\", ");
                        generateEXP(stmt->val.printExp, outputFile);
                        fprintf(outputFile, ");\n");
                        break;
                case t_string:
                        fprintf(outputFile, "printf(\"%%s\", ");
                        generateEXP(stmt->val.printExp, outputFile);
                        fprintf(outputFile, ");\n");
                        break;
                case t_bool:
                        fprintf(outputFile, "printf(\"%%d\", ");
                        generateEXP(stmt->val.printExp, outputFile);
                        fprintf(outputFile, ");\n");
                        break;
                }

                break;
        case k_stmtKindAssign:
                fprintf(outputFile, "%s = ", stmt->val.assignStmt.identifier);
                generateEXP(stmt->val.assignStmt.rhs, outputFile);
                fprintf(outputFile, ";\n");
                break;
	}

	if (stmt->nextStmt != NULL)
	{
		generateSTMT(stmt->nextStmt, outputFile);
	}
}


void generateEXP(EXP *exp, FILE *outputFile){
	switch (exp->kind) {
        case k_expressionKindIdentifier:
                fprintf(outputFile, "%s", exp->val.identifier);
                break;

        case k_expressionKindStringVal:
                fprintf(outputFile, "%s", exp->val.stringval);
                break;

        case k_expressionKindIntVal:
                fprintf(outputFile, "%i", exp->val.intval);
                break;

	case k_expressionKindFloatVal:
                fprintf(outputFile, "%f", exp->val.floatval);
                break;

	case k_expressionKindBoolVal:
                exp->val.boolval ? fprintf(outputFile, "true") : fprintf(outputFile, "false");
                break;

        case k_expressionKindAddition:
                fprintf(outputFile, "(");
                generateEXP(exp->val.binary.lhs, outputFile);
                fprintf(outputFile, " + ");
                generateEXP(exp->val.binary.rhs, outputFile);
                fprintf(outputFile, ")");
                break;

        case k_expressionKindSubtraction:
                fprintf(outputFile, "(");
                generateEXP(exp->val.binary.lhs, outputFile);
                fprintf(outputFile, " - ");
                generateEXP(exp->val.binary.rhs, outputFile);
                fprintf(outputFile, ")");
                break;

        case k_expressionKindMultiplication:
                fprintf(outputFile, "(");
                generateEXP(exp->val.binary.lhs, outputFile);
                fprintf(outputFile, " * ");
                generateEXP(exp->val.binary.rhs, outputFile);
                fprintf(outputFile, ")");
                break;

        case k_expressionKindDivision:
                fprintf(outputFile, "(");
                generateEXP(exp->val.binary.lhs, outputFile);
                fprintf(outputFile, " / ");
                generateEXP(exp->val.binary.rhs, outputFile);
                fprintf(outputFile, ")");
                break;

        case k_expressionKindEquals:
                fprintf(outputFile, "(");
                generateEXP(exp->val.binary.lhs, outputFile);
                fprintf(outputFile, " == ");
                generateEXP(exp->val.binary.rhs, outputFile);
                fprintf(outputFile, ")");
                break;

        case k_expressionKindNotEquals:
                fprintf(outputFile, "(");
                generateEXP(exp->val.binary.lhs, outputFile);
                fprintf(outputFile, " != ");
                generateEXP(exp->val.binary.rhs, outputFile);
                fprintf(outputFile, ")");
                break;

        case k_expressionKindAnd:
                fprintf(outputFile, "(");
                generateEXP(exp->val.binary.lhs, outputFile);
                fprintf(outputFile, " && ");
                generateEXP(exp->val.binary.rhs, outputFile);
                fprintf(outputFile, ")");
                break;

        case k_expressionKindOr:
                fprintf(outputFile, "(");
                generateEXP(exp->val.binary.lhs, outputFile);
                fprintf(outputFile, " || ");
                generateEXP(exp->val.binary.rhs, outputFile);
                fprintf(outputFile, ")");
                break;

        case k_expressionKindUMinus:
                fprintf(outputFile, "-");
                generateEXP(exp->val.unary.child, outputFile);
                break;

        case k_expressionKindBang:
                fprintf(outputFile, "!");
                generateEXP(exp->val.unary.child, outputFile);
                break;
        }
}
