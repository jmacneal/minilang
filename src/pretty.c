#include <stdio.h>
#include "pretty.h"

int indentation;

void prettyPROG(PROG *p)
{
        indentation = 0;
        if(p->rootDeclaration != NULL)
                prettyDEC(p->rootDeclaration);

        if(p->rootStatement != NULL){
                prettySTMT(p->rootStatement);
        }
}

void prettyDEC(DEC *d)
{
	switch (d->type->val)
	{
		case t_int:
			printf("var %s : int = ", d->identifier);
                        prettyEXP(d->rhs);
                        printf(";\n");
			break;
		case t_float:
			printf("var %s : float = ", d->identifier);
                        prettyEXP(d->rhs);
                        printf(";\n");
			break;
		case t_string:
			printf("var %s : string = ", d->identifier);
                        prettyEXP(d->rhs);
                        printf(";\n");
			break;
		case t_bool:
			printf("var %s : boolean = ", d->identifier);
                        prettyEXP(d->rhs);
                        printf(";\n");
			break;
	}

	if (d->nextDec != NULL)
	{
		prettyDEC(d->nextDec);
	}
}

void prettySTMT(STMT *s)
{
        for(int i=0; i<indentation; i++)
                printf("   ");
	switch (s->kind)
	{
		case k_stmtKindIf:
			printf("if ");
			prettyEXP(s->val.ifStmt.conditionExp);
			printf(" {\n");
                        indentation++;
			prettySTMT(s->val.ifStmt.bodyStmt);
                        indentation--;
                        for(int i=0; i<indentation; i++) printf("   ");
                        printf("}\n");
			break;
		case k_stmtKindIfElse:
			printf("if ");
			prettyEXP(s->val.ifelseStmt.conditionExp);
			printf(" {\n");
                        indentation++;
			prettySTMT(s->val.ifelseStmt.bodyStmt);
                        indentation--;
                        for(int i=0; i<indentation; i++) printf("   ");
                        printf("}\n");
                        for(int i=0; i<indentation; i++) printf("   ");
			printf("else {\n");
                        indentation++;
			prettySTMT(s->val.ifelseStmt.elseStmt);
                        indentation--;
                        for(int i=0; i<indentation; i++) printf("   ");
			printf("}\n");
			break;
		case k_stmtKindWhile:
			printf("while ");
			prettyEXP(s->val.whileStmt.conditionExp);
			printf(" {\n");
                        indentation++;
			prettySTMT(s->val.whileStmt.bodyStmt);
                        indentation--;
                        for(int i=0; i<indentation; i++) printf("   ");
			printf("}\n");
			break;
		case k_stmtKindRead:
			printf("read %s;\n", s->val.readId);
			break;
		case k_stmtKindPrint:
			printf("print ");
			prettyEXP(s->val.printExp);
			printf(";\n");
			break;
		case k_stmtKindAssign:
			printf("%s = ", s->val.assignStmt.identifier);
			prettyEXP(s->val.assignStmt.rhs);
			printf(";\n");
			break;
	}

	if (s->nextStmt != NULL)
	{
		prettySTMT(s->nextStmt);
	}
}

void prettyEXP(EXP *e)
{
	switch (e->kind) {
        case k_expressionKindIdentifier:
                printf("%s", e->val.identifier);
                break;

        case k_expressionKindStringVal:
                printf("%s", e->val.stringval);
                break;

        case k_expressionKindIntVal:
                printf("%i", e->val.intval);
                break;

	case k_expressionKindFloatVal:
                printf("%f", e->val.floatval);
                break;

	case k_expressionKindBoolVal:
                e->val.boolval ? printf("TRUE") : printf("FALSE");
                break;

        case k_expressionKindAddition:
                printf("(");
                prettyEXP(e->val.binary.lhs);
                printf(" + ");
                prettyEXP(e->val.binary.rhs);
                printf(")");
                break;

        case k_expressionKindSubtraction:
                printf("(");
                prettyEXP(e->val.binary.lhs);
                printf(" - ");
                prettyEXP(e->val.binary.rhs);
                printf(")");
                break;

        case k_expressionKindMultiplication:
                printf("(");
                prettyEXP(e->val.binary.lhs);
                printf(" * ");
                prettyEXP(e->val.binary.rhs);
                printf(")");
                break;

        case k_expressionKindDivision:
                printf("(");
                prettyEXP(e->val.binary.lhs);
                printf(" / ");
                prettyEXP(e->val.binary.rhs);
                printf(")");
                break;

        case k_expressionKindEquals:
                printf("(");
                prettyEXP(e->val.binary.lhs);
                printf(" == ");
                prettyEXP(e->val.binary.rhs);
                printf(")");
                break;

        case k_expressionKindNotEquals:
                printf("(");
                prettyEXP(e->val.binary.lhs);
                printf(" != ");
                prettyEXP(e->val.binary.rhs);
                printf(")");
                break;

        case k_expressionKindAnd:
                printf("(");
                prettyEXP(e->val.binary.lhs);
                printf(" && ");
                prettyEXP(e->val.binary.rhs);
                printf(")");
                break;

        case k_expressionKindOr:
                printf("(");
                prettyEXP(e->val.binary.lhs);
                printf(" || ");
                prettyEXP(e->val.binary.rhs);
                printf(")");
                break;

        case k_expressionKindUMinus:
                printf("-");
                break;

        case k_expressionKindBang:
                printf("!");
                break;
        }
}
