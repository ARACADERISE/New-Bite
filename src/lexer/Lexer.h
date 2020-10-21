#ifndef Lexer
#define Lexer
#include <stdlib.h>
#include <string.h>
#include "tokens.h"
#include <ctype.h>
#include "keyword_lexer/keyword_lexer.h"

typedef struct lexer {
    char current_char;
    char last_char; // this will be used if we have an error
    char* source_code;
    int index;
    int line;
    //Tokens_* tokens;
    union {
        char* current_keyword;
    } *KEY_VALUES;
    struct KeywordLexer* keyword_lexer;
    Tokens_* tokens;
} lexer_;

lexer_* init_lexer(char* src_code);
void move_pointer(lexer_* lexer);
Tokens_* get_next_token(lexer_* lexer);

#endif