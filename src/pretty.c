#include <stdio.h>
#include "pretty.h"

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
                printf("+");
                prettyEXP(e->val.binary.rhs);
                printf(")");
                break;

        case k_expressionKindSubtraction:
                printf("(");
                prettyEXP(e->val.binary.lhs);
                printf("-");
                prettyEXP(e->val.binary.rhs);
                printf(")");
                break;

        case k_expressionKindMultiplication:
                printf("(");
                prettyEXP(e->val.binary.lhs);
                printf("*");
                prettyEXP(e->val.binary.rhs);
                printf(")");
                break;

        case k_expressionKindDivision:
                printf("(");
                prettyEXP(e->val.binary.lhs);
                printf("/");
                prettyEXP(e->val.binary.rhs);
                printf(")");
                break;

        case k_expressionKindEquals:
                printf("(");
                prettyEXP(e->val.binary.lhs);
                printf("=");
                prettyEXP(e->val.binary.rhs);
                printf(")");
                break;

        case k_expressionKindNotEquals:
                printf("(");
                prettyEXP(e->val.binary.lhs);
                printf("!=");
                prettyEXP(e->val.binary.rhs);
                printf(")");
                break;

        case k_expressionKindAnd:
                printf("(");
                prettyEXP(e->val.binary.lhs);
                printf("&&");
                prettyEXP(e->val.binary.rhs);
                printf(")");
                break;

        case k_expressionKindOr:
                printf("(");
                prettyEXP(e->val.binary.lhs);
                printf("||");
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
