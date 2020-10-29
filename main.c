#include <stdio.h>
#include "src/read_file.h"
#include "src/lexer/Lexer.h"
#include "src/parser/parser.h"
#include "src/lexer/tokens.h"
#include "src/SyntaxTree/syntax_tree.h"

/*
    Bite is a featured low-level language that supports built-in assembly code.(x86 AT&T syntax)

    The bite language supports high-level features, such as the forEach function, but transfers it down into low-level x86 assembly code

    In the end, you're given a low-level, assembly friendly language that supports high-level features with fast runtime speeds!
*/

int main(int args, char* argv[]) {
    
    if(args < 2) {
        fprintf(stderr,"\nPlease specify a Bite file to compile.\n\n");
        exit(1);
    }

    //file_check_extension(argv[1],".b");
    char* code = read_file(argv[1]);
    lexer_* lex = init_lexer(code);
    //Tokens_* tokens = (void*)0;
    Parser_* parser = init_parser(lex);
    SyntaxTree_* lang_tree = parse_bite(parser);

    COMPILE(lang_tree);
    return 0;
}