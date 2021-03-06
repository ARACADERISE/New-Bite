#ifndef Tokens
#define Tokens
#include <stdlib.h>
#include <string.h>

typedef struct Tokens {
    char* token_value;
    enum {
        Token_id, // variable names, function names etc disregarding the main function
        Token_func_keyword,
        Token_Print_Function,
        Token_LP,
        Token_RP,
        Token_LT,
        Token_GT,
        Token_Colon,
        Token_Comma,
        Token_Equals,
        Token_LC, // {
        Token_RC, // }
        Token_return,
        Token_semicolon,
        Token_Int,
        Token_String,
        Token_Array,
        Token_Void,
        Token_Use,
        Token_Make,
        Token_SingleQuotes,
        Token_DoubleQuotes,
        Token_eof
    } TokenType;
    int main_was_found; // helper variable for now. Will be removed when the compiler is implemented
} Tokens_;

Tokens_* setup_token(int token_id, char* value);
Tokens_* configure_current_keyword(char* keyword);

#endif