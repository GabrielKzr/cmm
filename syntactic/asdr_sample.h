/*
 * @file    syntactic/asdr_sample.c
 * @brief   This file contains the entry point for the recursive descent parser for the C-- example language.
 * @author  Gabriel Kaizer de Menezes & Nathan da Rosa Cidal
 */

#ifndef ASDR_SAMPLE_H
#define ASDR_SAMPLE_H

#include <stdbool.h>
#include <stdint.h>

enum {
    TEOF = 0,
    WHILE = 256,
    IF,
    ELSE,
    IDENT,
    NUM,
    TYPE,
    VOID,
    FUNC,
    // RETURN,
    // REL_OP,
};

void prog();

extern bool debug;

#endif