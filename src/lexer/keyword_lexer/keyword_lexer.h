#ifndef keywordLexer
#define keywordLexer
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../Lexer.h"

typedef struct KeywordLexer {
    struct lexer* lexer; // storing keywords in this
} KeywordLexer_;

KeywordLexer_* init_key_lexer();
struct lexer* gather_keyword(struct lexer* lexer, KeywordLexer_* key_lexer);

#endif