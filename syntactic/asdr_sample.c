#include "asdr_sample.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

bool debug = false;
int pline = 0;

extern int yylineno;
extern int yylex(void);
extern char *yytext;
extern 

void setDebug(bool _debug)
{
    debug = _debug;
}

void yyerror(const char *s, ...)
{
    va_list args;
    va_start(args, s);
    vfprintf(stderr, s, args);
    va_end(args);
    exit(EXIT_FAILURE);
}

static const char *tokenName(int token)
{
    switch (token) {
        case FUNC:   return "FUNC";
        case TYPE:   return "TYPE";
        case VOID:   return "VOID";
        case IDENT:  return "IDENT";
        case NUM:    return "NUM";
        case IF:     return "IF";
        case ELSE:   return "ELSE";
        case WHILE:  return "WHILE";
        // case RETURN: return "RETURN";
        case TEOF:    return "EOF";
        default:
            if (token >= 32 && token < 127) {
                static char buf[2];
                buf[0] = (char)token;
                buf[1] = '\0';
                return buf;
            }
            return "?";
    }
}
 
#define DEBUG_PRINT_TOKEN(t)                    \
    do {                                        \
        if (debug) {                            \
            if (yylineno != pline)              \
                printf("\n");                   \
            printf("%s ", tokenName(t));        \
            pline = yylineno;                   \
        }                                       \
    } while (0)

static int laToken;

static void listaDecl(void);
static void restoDecl(void);

static void check(int token);
static void declVar(void);
static void restoFunc(void);
static void listaIdent(void);

static void paramList(void);

static void listaCmd(void);
static void cmd(void);
static void restoIf(void);
static void bloco(void);
static void formalPar(void);
static void E(void);
static void T(void);
static void F(void);

static void check(int token)
{
    if (laToken == token) {
        if(laToken != TEOF)
        {
            DEBUG_PRINT_TOKEN(token);
            laToken = yylex();
        }
    }
    else
    {
        yyerror("Error: expected token %d, but got %d\n", token, laToken);
    }
}

static void listaIdent(void)
{   
    if(laToken == IDENT)
    {
        check(IDENT);
        
        if(laToken == ',')
        {
            check(',');
            listaIdent();
        }
        else
        {
            (void)0; // everithing fine, do nothing, this is the case where there is no more identifiers
        }
    }
    else
    {
        yyerror("Error: expected IDENT, but got %d\n", laToken);
    }
}

static void declVar(void)
{
    check(TYPE);
    listaIdent();
    check(';');

    if(laToken == TYPE)
    {
        declVar();
    }
    else
    {
        (void)0; // everithing fine, do nothing, this is the case where there is no more variable declarations
    }
}

static void paramList(void)
{
    check(TYPE);
    check(IDENT);

    if(laToken == ',')
    {
        check(',');
        paramList();
    }
    else
    {
        (void)0; // everithing fine, do nothing, this is the case where there is no more parameters
    }
}

static void bloco(void)
{
    check('{');
    listaCmd();
    check('}');
}

static void F(void)
{
    if(laToken == IDENT)
    {
        check(IDENT);
    }
    else if(laToken == NUM)
    {
        check(NUM);
    }
    else if(laToken == '(')
    {
        check('(');
        E();
        check(')');
    }
    else
    {
        yyerror("Error: expected IDENT or NUM or '(' %d\n", laToken);
    }
}

static void T(void)
{
    F();
    if(laToken == '*' || laToken == '/')
    {
        check(laToken);
        T();
    }
    else
    {
        (void)0; // everithing fine, do nothing, this is the case where there is no more terms
    }
}

static void E(void)
{
    T();
    if(laToken == '+' || laToken == '-')
    {
        check(laToken);
        E();
    }
    else
    {
        (void)0; // everithing fine, do nothing, this is the case where there is no more expressions
    }
}

static void restoIf(void)
{
    if(laToken == ELSE)
    {
        check(ELSE);
        cmd();
    }
    else
    {
        (void)0; // everithing fine, do nothing, this is the case where there is no else
    }
}

static void cmd(void)
{
    if(laToken == '{')
    {
        bloco();
    }
    else if(laToken == WHILE)
    {
        check(WHILE);
        check('(');
        E();
        check(')');
        cmd();
    }
    else if(laToken == IDENT)
    {
        check(IDENT);
        check('=');
        E();
        check(';');
    }
    else if(laToken == IF)
    {
        check(IF);  
        check('(');
        E();
        check(')');
        cmd();
        restoIf();
    }
    else
    {
        yyerror("Error: expected '{' or WHILE or IDENT or IF at line %d\n", yylineno);
    }
}

static void listaCmd(void)
{
    if (laToken == '}') {
        (void)0; // everithing fine, do nothing, this is the case where there is no more commands
    }
    else
    {
        cmd();
        listaCmd();
    }
}

static void formalPar(void)
{
    if(laToken == TYPE)
    {
        paramList();
    }
    else
    {
        (void)0; // everithing fine, do nothing, this is the case where there is no parameters
    }
}

/*
static void declFunc(void)
{   
    check(FUNC);

    tipoOuVoid();

    check(IDENT);
    check('(');

    formalPar();

    check(')');
    check('{');

    if(laToken == TYPE)
    {
        declVar();
    }

    listaCmd();

    check('}');

    if(laToken == FUNC)
    {
        declFunc();
    }
    else
    {
        (void)0; // everithing fine, do nothing, this is the case where there is no more function declarations
    }
}
*/

static void restoFunc(void)
{
    check('(');
    formalPar();
    check(')');
    check('{');
    declVar();
    listaCmd();
    check('}');
    listaDecl();
}

static void restoDecl(void)
{
    if(laToken == ';')
    {
        check(';');
        listaDecl();
    }
    else if(laToken == ',')
    {
        check(',');
        listaIdent();
        check(';');
        listaDecl();
    }
    else if(laToken == '(')
    {
        restoFunc();
    }
    else 
    {
        yyerror("Error: expected ';' or '(' at line %d\n", yylineno);
    }
}

static void listaDecl(void)
{
    if(laToken == FUNC)
    {
        check(FUNC); // not used anymore, just legacy
        printf("legacy ");
    }

    if(laToken == TYPE)
    {
        check(TYPE);
        check(IDENT);
        restoDecl();        
    }
    else if(laToken == VOID)
    {
        check(VOID);
        check(IDENT);
        restoFunc();
    }
    else if(laToken == TEOF)
    {
        (void)0; // everithing fine, do nothing, this is the case where there is no more declarations
    }
    else
    {
        yyerror("Error: expected TYPE or VOID or EOF at line", yylineno);
    }
}

void prog(void)
{
    laToken = yylex();

    listaDecl();
    check(TEOF);
    printf("\nEOF\n");
}