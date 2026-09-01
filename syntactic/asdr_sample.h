// asdr_sample.h

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

// void syntatic_asdr(void *list, uint8_t (*pop)(void *, void *));

void prog();

extern bool debug;

#endif