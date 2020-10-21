#include "keyword_lexer.h"
#include <stdio.h>

/*
    This is part of the lexer.
    It helps pickup keywords so the main lexer file does not become clustered.
*/

KeywordLexer_* init_key_lexer(lexer_* lexer) {
    KeywordLexer_* key_lex = calloc(1,sizeof(*key_lex));

    key_lex->lexer = lexer; // we will append values picked up by the keyword lexer to the actual lexer itself

    return key_lex;
}

static char* convert_to_string(char c) {
    char* val = calloc(2,sizeof(char*));

    val[0] = c;
    val[1] = '\0';
    return val;
}

struct lexer* gather_keyword(lexer_* lexer, KeywordLexer_* key_lexer) {

    char* keyword = calloc(1,sizeof(char));
    do {
        // ToDo: Pickup Keywords and store them in the actual lexer
        char* curr = convert_to_string(lexer->current_char);

        keyword = realloc(
            keyword,
            (strlen(keyword)+2)*sizeof(char*)
        );
        strcat(keyword,curr);
        move_pointer(lexer);

        if(
            !(isalnum(lexer->current_char)) && !(lexer->current_char == '_')
        ) break;
    } while(1);
    
    lexer->tokens = configure_current_keyword(keyword);

    return lexer;
}