/*
 * @file    lexer/lexer.flex
 * @brief   This file contains the implementation of a lexical analyzer for the C-- example language.
 * @details It defines patterns for various tokens such as keywords, identifiers, numbers, and operators.
 *          The lexer uses Flex to generate C code that recognizes these tokens and provides error handling for invalid characters.
 * @author  Gabriel Kaizer de Menezes & Nathan da Rosa Cidal
 */

%{
#include "asdr_sample.h"
#include <stdbool.h>

extern void setDebug(bool debug);
%}

%option noinput
%option nounput
%option noyywrap
%option yylineno
%option yylineno

WHITE_SPACE_CHAR [\n\r\ \t\b]

%%

"\$TRACE_ON"    { setDebug(true); }
"\$TRACE_OFF"   { setDebug(false); }

"while"         { return WHILE; }
"if"            { return IF; }
"else"          { return ELSE; }

"int"     |
"double"  |
"boolean" |
"bool"          { return TYPE; }
"void"          { return VOID; }
"func"          { return FUNC; }

"return"        { printf("Erro lexico: palavra reservada 'return' nao permitida\n"); }

"==" |
"!=" |
"<=" |
">=" |
"<"  |
">"                     { printf("Erro lexico: operador relacional '%s' nao permitido\n", yytext); }

"true"                    { return NUM; }
"false"                   { return NUM; }

[a-zA-Z_][a-zA-Z0-9_]*    { return IDENT; }

[0-9]+(\.[0-9]+)?         { return NUM; }

"{" |
"}" |
";" |
"(" |
")" |
"+" |
"-" |
"=" |
"," |
"*" |
"/"                  { return yytext[0]; }

{WHITE_SPACE_CHAR}+       ;// { /* ignora */ }

. {
    printf("Erro lexico: caracter invalido: <%s>\n", yytext);
}

%%