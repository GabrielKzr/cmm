#include "asdr_sample.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

bool debug = false;

extern int yylex(void);
extern char *yytext;

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
 
#define DEBUG_PRINT_TOKEN(t) \
    do { if (debug) printf("%s\n", tokenName(t)); } while (0)


/*
  Prog -->  ListaDecl

  ListaDecl -->  DeclVar  ListaDecl
              |  DeclFun  ListaDecl
              |  // vazio

  DeclVar --> Tipo ListaIdent ';' DeclVar
            | // vazio

  Tipo --> int | double | boolean

  ListaIdent --> IDENT , ListaIdent  
               | IDENT      

  DeclFun --> FUNC tipoOuVoid IDENT '(' FormalPar ')' '{' DeclVar ListaCmd '}' DeclFun
            | // vazio

  TipoOuVoid --> Tipo | VOID

  // Formal par é inútil, a análise pode ser feita sem ele e ir direto pra paramlist e tratar da mesma forma que lista Ident
  // caso contrário, é necessário analisar (vazio) que não significa nada
  FormalPar -> paramList  
            | // vazio

  paramList --> Tipo IDENT , ParamList
              | Tipo IDENT 

  Bloco --> { ListaCmd }

  ListaCmd --> Cmd ListaCmd
    |    // vazio

  Cmd --> Bloco
      | while ( E ) Cmd
      | IDENT = E ;
      | if ( E ) Cmd RestoIf

  RestoIf -> else Cmd
        |    // vazio
  E --> E + T
      | E - T
      | T

  T --> T * F
      | T / F
      | F    
      
  F -->  IDENT
      | NUM
      | ( E )
*/

static int laToken;

static void check(int token);
static void declVar(void);
static void listaIdent(void);

static void declFunc(void);
static void tipoOuVoid(void);
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
        DEBUG_PRINT_TOKEN(token);
        if(laToken != TEOF)
            laToken = yylex();
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

static void tipoOuVoid(void)
{
    if(laToken == TYPE)
    {
        check(TYPE);
    }
    else if(laToken == VOID)
    {
        check(VOID);
    }
    else
    {
        yyerror("Error: expected TYPE or VOID, but got %d\n", laToken);
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
        yyerror("Error: unexpected token %d\n", laToken);
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
        yyerror("Error: unexpected token %d\n", laToken);
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

static void listaDecl(void)
{
    if(laToken == TYPE)
    {
        declVar();
        listaDecl();
    }
    else if(laToken == FUNC)
    {
        declFunc();
        listaDecl();
    }
    else if(laToken == TEOF)
    {
        (void)0; // everithing fine, do nothing, this is the case where there is no more declarations
    }
    else
    {
        yyerror("Error: unexpected token %d\n", laToken);
    }
}

void prog(void)
{
    laToken = yylex();

    listaDecl();
    check(TEOF);
    printf("\nEOF\n");
}