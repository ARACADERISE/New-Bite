#include "Lexer.h"
#include <stdio.h>

lexer_* init_lexer(char* src_code) {
    lexer_* lexer = calloc(1,sizeof(*lexer));

    lexer->index = 0;
    lexer->line = 1; // 1 by default, there is no such thing as line 0
    lexer->source_code = src_code;
    lexer->current_char = lexer->source_code[lexer->index];
    lexer->last_char = lexer->current_char;
    lexer->KEY_VALUES = calloc(1,sizeof(*lexer->KEY_VALUES)); // default memory, nothing needed yet for this union pointer
    lexer->tokens = calloc(1,sizeof(*lexer->tokens));
    lexer->keyword_lexer = init_key_lexer();

    return lexer;
}


void move_pointer(lexer_* lexer) {
    if(lexer->index < strlen(lexer->source_code) && !(lexer->current_char == '\0')) {

        lexer->last_char = lexer->current_char;
        lexer->index++;
        lexer->current_char = lexer->source_code[lexer->index];

        if(lexer->current_char == '\n')
            lexer->line++;
    }
}

static void move_over_whitespace(lexer_* lexer) {
    do{
        move_pointer(lexer);
    } while(lexer->current_char == ' ' || lexer->current_char == '\t' || lexer->current_char == 10);
}

static Tokens_* move_pointer_with_token(char* value,int TokenId, lexer_* lexer) {
    lexer->tokens = setup_token(TokenId,value);
    move_pointer(lexer);
    return lexer->tokens;
}

Tokens_* get_next_token(lexer_* lexer) {
    do {

        if(lexer->current_char == ' ' || lexer->current_char == 10)
            move_over_whitespace(lexer);

        if(lexer->current_char == '\n')
            move_pointer(lexer);
        
        if(isalnum(lexer->current_char)) {
            gather_keyword(lexer,lexer->keyword_lexer);

            return lexer->tokens;
        }

        switch(lexer->current_char) {
            case '(': return move_pointer_with_token("(",Token_LP,lexer);break;
            case ')': return move_pointer_with_token(")",Token_RP,lexer);break;
            case ':': return move_pointer_with_token(":",Token_Colon,lexer);break;
            case '<': return move_pointer_with_token("<",Token_LT,lexer);break;
            case '>': return move_pointer_with_token(">",Token_GT,lexer);break;
            case '{': return move_pointer_with_token("{",Token_LC,lexer);break;
            case '}': return move_pointer_with_token("}",Token_RC,lexer);break;
            case ';': return move_pointer_with_token(";",Token_semicolon,lexer);
            case ',': return move_pointer_with_token(",",Token_Comma,lexer);
            case '=': return move_pointer_with_token("=",Token_Equals,lexer);
            case '\0': break;
            default: {
                fprintf(stderr,"\nUknown character `%c` caught on line %d\n",lexer->current_char,lexer->line);
                exit(1);
            }
        }
            
    } while(lexer->current_char != '\0');

    return setup_token(Token_eof,"\0");
}