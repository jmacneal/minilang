// Base tree structure for AST (base copied from tinylang)

#ifndef TREE_H
#define TREE_H

/* Main program definition: a linked list of declarations
   and a linked lits of statements*/
typedef struct PROG PROG;
typedef struct DEC DEC;
typedef struct STMT STMT;
typedef struct EXP EXP;
typedef struct TYPE TYPE;

typedef enum Type {t_float,
              t_int,
              t_string,
              t_bool
} Type;

struct TYPE{
        Type val;
};
TYPE *makeTYPE_int();
TYPE *makeTYPE_float();
TYPE *makeTYPE_string();
TYPE *makeTYPE_bool();

struct PROG {
        DEC *rootDeclaration; // Head (1st node) of dec linked list
        /* DEC *nextDeclaration; // 1 past Tail (last node) of dec linked list */
        STMT *rootStatement; // Head (1st node) of stmt linked list
        /* STMT *nextStatement; // 1 past Tail (last node) of stmt linked list */
};
PROG *makePROG(DEC *rootDeclaration, STMT *rootStatement);

/* Declaration struct, with nextDeclaration pointers
forming a linked (declaration)list */
struct DEC {
        int lineno;
        char *identifier;
        TYPE *type;
        EXP *rhs;
        DEC *nextDec;
};
DEC *insertDEC(DEC *decList, DEC *dec);
DEC *makeDEC(TYPE *type, char *identifier, EXP *rhs);


/* Statement struct, with nextStatement pointers
forming a linked (statement)list */
typedef enum {k_stmtKindAssign,
              k_stmtKindIf,
              k_stmtKindIfElse,
              k_stmtKindWhile,
              k_stmtKindPrint,
              k_stmtKindRead
        } StatementKind;
struct STMT {
        int lineno;

        StatementKind kind;

        union{
                struct {char *identifier; EXP *rhs;} assignStmt;
                struct {EXP *conditionExp; STMT *bodyStmt;} ifStmt;
                struct {EXP *conditionExp;
                        STMT *bodyStmt;
                        STMT *elseStmt;} ifelseStmt;
                struct {EXP *conditionExp; STMT *bodyStmt;} whileStmt;
                EXP *printExp;
                char *readId;
        } val;

        STMT *nextStmt;
};
STMT *insertSTMT(STMT *stmtList, STMT *stmt);
STMT *makeSTMT_assign(const char *identifier, EXP *rhs);
STMT *makeSTMT_if(EXP *conditionExp, STMT *bodyStmt);
STMT *makeSTMT_ifelse(EXP *conditionExp, STMT *bodyStmt, STMT *elseStmt);
STMT *makeSTMT_while(EXP *conditionExp, STMT *bodyStmt);
STMT *makeSTMT_print(EXP *printExp);
STMT *makeSTMT_read(char *readId);


typedef enum {
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

struct EXP {
	int lineno;

        ExpressionKind kind;
        enum Type type;
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
EXP *makeEXP_identifier(const char *identifier);
EXP *makeEXP_stringval(const char *stringval);
EXP *makeEXP_intval(int intval);
EXP *makeEXP_floatval(float floatval);
EXP *makeEXP_boolval(int boolval);
EXP *makeEXP_unary(ExpressionKind op, EXP *unary);
EXP *makeEXP_binary(ExpressionKind op, EXP *lhs, EXP *rhs);


#endif /* !TREE_H */
