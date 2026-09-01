#ifndef LEXER_H_
#define LEXER_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "asdr_sample.h"

bool lexer_generate(void *list, uint8_t (*push)(void *, void *));

#endif /* LEXER_H_ */