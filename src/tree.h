// Base tree structure for AST (base copied from tinylang)

#ifndef TREE_H
#define TREE_H


/* Main program definition: a linked list of declarations
   and a linked lits of statements*/
typedef struct PROG PROG;
struct PROG {
        DEC *rootDeclaration; // Head (1st node) of dec linked list
        DEC *nextDeclaration; // 1 past Tail (last node) of dec linked list
        STMT *rootStatement; // Head (1st node) of stmt linked list
        STMT *nextStatement; // 1 past Tail (last node) of stmt linked list
};
PROG *makePROG();
void *insertDEC(DEC *dec);
void *insertSTMT(STMT *stmt);

/* Declaration struct, with nextDeclaration pointers
forming a linked (declaration)list */
typedef struct DEC DEC;
struct DEC {
        int lineno;

        char *identifier;

        enum {t_float,
              t_int,
              t_string,
              t_bool
        } type;

        EXP *rhs;

        DEC *nextDec;
};
DEC *makeDEC_assignInt(char *identifier, EXP *rhs, DEC *nextDec);
DEC *makeDEC_assignFloat(char *identifier, EXP *rhs, DEC *nextDec);
DEC *makeDEC_assignString(char *identifier, EXP *rhs, DEC *nextDec);
DEC *makeDEC_assignBool(char *identifier, EXP *rhs, DEC *nextDec);

/* Statement struct, with nextStatement pointers
forming a linked (statement)list */
typedef struct STMT STMT;
struct STMT {
        int lineno;

        enum {k_stmtKindAssign,
              k_stmtKindIf,
              k_stmtKindIfElse,
              k_stmtKindWhile,
              k_stmtKindPrint,
              k_stmtKindRead} StmtKind;

        union{
                struct {char *identifier;
                        EXP *exp;} assignStmt;
                struct {EXP *conditionExp;
                        STMT *bodyStmt;} ifStmt;
                struct {EXP *conditionExp;
                        STMT *bodyStmt;
                        STMT *elseStmt;} ifelseStmt;
                struct {EXP *conditionExp;
                        STMT *bodyStmt;} whileStmt;
                EXP *printExp;
                char *readId;
        } val;

        STMT *nextStmt;
};
STMT *makeSTMT_assign(char *identifier, EXP *exp);
STMT *makeSTMT_if(EXP *conditionExp, STMT *bodyStmt);
STMT *makeSTMT_ifelse(EXP *conditionExp, STMT *bodyStmt, STMT *elseStmt);
STMT *makeSTMT_while(EXP *conditionStmt, STMS *bodyStmt);
STMT *makeSTMT_print(EXP *printExp);
STMT *makeSTMT_read(char *readId);

typedef struct EXP EXP;
struct EXP {
	int lineno;

        enum {
                k_expressionKindIdentifier,
                k_expressionKindStringVal,
                k_expressionKindIntVal,
                k_expressionKindFloatVal,
                k_expressionKindBoolVal,
                k_expressionKindAddition,
                k_expressionKindSubtraction,
                k_expressionKindMultiplication,
                k_expressionKindDivision,
                k_expressionKindEquals,
                k_expressionKindNotEquals,
                k_expressionKindAnd,
                k_expressionKindOr,
                k_expressionKindUMinus,
                k_expressionKindBang
        } ExpressionKind;

	union {
		char *identifier;
                char *stringval;
		int intval;
                float floatval;
                int boolval;
		struct { EXP *lhs; EXP *rhs; } binary;
                struct { EXP *child; } unary;
	} val;
};

EXP *makeEXP_identifier(char *identifier);
EXP *makeEXP_stringval(char *stringval);
EXP *makeEXP_intval(int intval);
EXP *makeEXP_floatval(float floatval);
EXP *makeEXP_boolval(int boolval);
EXP *makeEXP_unary(ExpressionKind op, EXP *unary);
EXP *makeEXP_binary(ExpressionKind op, EXP *lhs, EXP *rhs);

#endif /* !TREE_H */
