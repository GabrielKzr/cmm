#include "lexer.h"

extern int yylex(void);
extern char *yytext;

bool lexer_generate(void *list, uint8_t (*push)(void *, void *))
{
    int token;

    while ((token = yylex()) != 0) {
        switch (token) {
            case WHILE:
                printf("WHILE");
                break;

            case IF:
                printf("IF");
                break;

            case ELSE:
                printf("ELSE");
                break;

            case VOID:
                printf("VOID");
                break;
            
            case FUNC:
                printf("FUNC");
                break;

            case RETURN:
                printf("RETURN");
                break;

            case IDENT:
                printf("IDENT");
                break;

            case REL_OP:
                printf("REL_OP");
                break;

            case TYPE:
                printf("TYPE");
                break;

            case NUM:
                printf("NUM");
                break;

            case '\n':
                printf("\n");
                continue;

            default:
                printf("%s", yytext);
                break;
        }

        printf(" ");

        int *token_ptr = malloc(sizeof(int));
        if (token_ptr == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            return false;
        }
        *token_ptr = token;

        push(list, token_ptr);
    }

    printf("\nEOF\n");

    return true;
}