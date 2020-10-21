#include <stdio.h>
#include "src/read_file.h"
#include "src/lexer/Lexer.h"
#include "src/parser/parser.h"
#include "src/lexer/tokens.h"
#include "src/SyntaxTree/syntax_tree.h"

int main(int args, char* argv[]) {
    
    if(args < 2) {
        fprintf(stderr,"\nPlease specify a Bite file to compile.\n\n");
        exit(1);
    }

    char* code = read_file(argv[1]);
    lexer_* lex = init_lexer(code);
    Tokens_* tokens = (void*)0;
    Parser_* parser = init_parser(lex);
    SyntaxTree_* lang_tree = parse_bite(parser);

    return 0;
}