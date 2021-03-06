#include "tokens.h"

Tokens_* setup_token(int token_id, char* value) {
    Tokens_* tokens = calloc(1,sizeof(*tokens));

    tokens->TokenType = token_id;
    tokens->token_value = value;

    return tokens;
}

Tokens_* configure_current_keyword(char* keyword) {
    if(strcmp(keyword,"fun")==0) return setup_token(Token_func_keyword,keyword);
    else if(strcmp(keyword,"Array")==0) return setup_token(Token_Array,keyword);
    else if(strcmp(keyword,"ret")==0) return setup_token(Token_return,keyword);
    else if(strcmp(keyword,"String")==0) return setup_token(Token_String,keyword);
    else if(strcmp(keyword,"Int")==0) return setup_token(Token_Int, keyword);
    else if(strcmp(keyword,"make")==0) return setup_token(Token_Make,keyword);
    else if(strcmp(keyword,"Print")==0) return setup_token(Token_Print_Function, keyword);
    else if(strcmp(keyword,"Void")==0) return setup_token(Token_Void,keyword);
    else if(strcmp(keyword,"use")==0) return setup_token(Token_Use,keyword);
    else return setup_token(Token_id, keyword);

    //else if(strcmp(keyword,"Print")==0) return setup_token(Token_Print_Function, keyword);
}