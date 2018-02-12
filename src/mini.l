/* The first section of a flex file contains:
 *    1. A code section for includes and other arbitrary C code. Copied to the scanner verbatim
 *    2. Helper definitions for regexes
 *    3. Scanner options
 */

/* Code section */
%{

#include "parser.tab.h" // Token definitions from the parser
#include <string.h>
#include <stdlib.h>
#include <math.h>

    extern int g_tokens;

    // Stores the line number with each token sent to bison
#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;
%}

/* Helper definitions */
DIGIT [0-9]
LETTER [a-zA-Z]
SYMBOL []_~@#$%&+/‘<>=|’.;:!?[{}(),^*'`-]
/* ' */
WS [ \t\r\n]
ESCAPE_CHAR [abfnrtv\\"]
/* " */


/* Scanner options, line number generation */
%option yylineno

/* Removes warnings about input/output functions */
%option noinput
%option nounput

%%

"\/\/".*
[ \t\r\n]+


TRUE    {if(g_tokens) printf("tTRUE\n"); return tTRUE;}
FALSE   {if(g_tokens) printf("tFALSE\n"); return tFALSE;}

int     {if(g_tokens) printf("tINT\n"); return tINT;}
float   {if(g_tokens) printf("tFLOAT\n"); return tFLOAT;}
boolean {if(g_tokens) printf("tBOOLEAN\n"); return tBOOLEAN;}
string  {if(g_tokens) printf("tSTRING\n"); return tSTRING;}

var     {if(g_tokens) printf("tVAR\n"); return tVAR;}
print   {if(g_tokens) printf("tPRINT\n"); return tPRINT;}
read    {if(g_tokens) printf("tREAD\n"); return tREAD;}

while   {if(g_tokens) printf("tWHILE\n"); return tWHILE;}
if      {if(g_tokens) printf("tIF\n"); return tIF;}
else    {if(g_tokens) printf("tELSE\n"); return tELSE;}

"="     {if(g_tokens) printf("tASSIGN\n"); return tASSIGN;}
"+"     {if(g_tokens) printf("tPLUS\n"); return tPLUS;}
"-"     {if(g_tokens) printf("tMINUS\n"); return tMINUS;}
"*"     {if(g_tokens) printf("tMULT\n"); return tMULT;}
"/"     {if(g_tokens) printf("tDIV\n"); return tDIV;}
"("     {if(g_tokens) printf("tLPAR\n"); return tLPAR;}
")"     {if(g_tokens) printf("tRPAR\n"); return tRPAR;}
"=="    {if(g_tokens) printf("tEQUALS\n"); return tEQUALS;}
"!="    {if(g_tokens) printf("tNOTEQUALS\n"); return tNOTEQUALS;}
"!"     {if(g_tokens) printf("tNOTEQUALS\n"); return tBANG;}
"&&"    {if(g_tokens) printf("tAND\n"); return tAND;}
"||"    {if(g_tokens) printf("tOR\n"); return tOR;}
":"     {if(g_tokens) printf("tCOLON\n"); return tCOLON;}
";"     {if(g_tokens) printf("tSEMICOLON\n"); return tSEMICOLON;}
"{"     {if(g_tokens) printf("tLBRACE\n"); return tLBRACE;}
"}"     {if(g_tokens) printf("tRBRACE\n"); return tRBRACE;}


0+{DIGIT}+ {
    fprintf(
            stderr, "Error: (line %d) Illegal Int '%s'\n",
            yylineno, yytext); exit(1);
         }

"."{DIGIT}+|{DIGIT}+"."|0+{DIGIT}+"."{DIGIT}+   {
    fprintf(
            stderr, "Error: (line %d) Illegal Float '%s'\n",
            yylineno, yytext); exit(1);
                                            }

0|([1-9]{DIGIT}*) {
    yylval.intval = atoi(yytext);
    if(g_tokens) printf("tINTVAL(%s)\n", yytext);
    return tINTVAL;
}

{DIGIT}+"."{DIGIT}* {
    yylval.floatval = atof(yytext);
    if(g_tokens) printf("tFLOATVAL(%s)\n", yytext);
    return tFLOATVAL;
                  }

\"({DIGIT}|{LETTER}|{SYMBOL}|{WS}|\\{ESCAPE_CHAR})*\" {
    yylval.stringval = strdup(yytext);
    if(g_tokens) printf("tSTRINGVAL(%s)\n", yytext);
    return tSTRINGVAL;
}


[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.identifier = strdup(yytext);
    if(g_tokens) printf("tIDENTIFIER(%s)\n", yytext);
    return tIDENTIFIER;
}

. 	{ fprintf (stderr, "Error: (line %d) unexpected character '%s'\n", yylineno, yytext); exit(1); }
%%
