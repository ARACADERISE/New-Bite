#ifndef PARSER
#define PARSER
#include <stdlib.h>
#include "../compiler/compiler.h"
#include "../lexer/tokens.h"
#include "../lexer/Lexer.h"
#include "../SyntaxTree/syntax_tree.h"

typedef struct Parser {
    Tokens_* curr_tokens;
    Tokens_* prev_tokens;
    lexer_* lexer;
} Parser_;

Parser_* init_parser(lexer_* lexer);
SyntaxTree_* parse_bite(Parser_* parser);

#endif