#include "parser.h"
#include <stdio.h>

Parser_* init_parser(lexer_* lexer) {
    Parser_* parser = calloc(1,sizeof(*parser));

    parser->lexer = lexer;
    parser->curr_tokens = get_next_token(parser->lexer);
    parser->prev_tokens = parser->curr_tokens;

    return parser;
}

static Parser_* move_token_pointer(Parser_* parser, int current_token) {
    if(parser->curr_tokens->TokenType == current_token) {
        parser->prev_tokens = parser->curr_tokens;
        parser->curr_tokens = get_next_token(parser->lexer);

        return parser;
    } else {
        fprintf(stderr,"\nThere was an error matching the token types\n\t(got %d,expected %d, line %d)\n",parser->curr_tokens->TokenType,current_token,parser->lexer->line);
        exit(1);
    }
}

static SyntaxTree_* parse_main_function(Parser_* parser) {
    move_token_pointer(parser,Token_id); // "main"

    if(parser->curr_tokens->TokenType == Token_semicolon) {
        fprintf(stderr,"\nError on line %d:\n\tCannot pre-define the main function\n", parser->lexer->line);
        exit(1);
    }

    move_token_pointer(parser, Token_LP);

    SyntaxTree_* main_func_tree = init_syntax_tree(Branch_MainFunction);
    
    if(parser->curr_tokens->TokenType == Token_id) {

        while(parser->curr_tokens->TokenType == Token_id) {

            if(main_func_tree->MainFunc_AmountOfArgs > 2) {
                fprintf(stderr,"\nToo many arguments for the main functions.\n\n");
                exit(1);
            }
            main_func_tree->MainFunc_AmountOfArgs++;
            main_func_tree->MainFunc_args = realloc(
                main_func_tree->MainFunc_args,
                (main_func_tree->MainFunc_AmountOfArgs+1)*sizeof(main_func_tree->MainFunc_args)
            );
            main_func_tree->MainFunc_args[main_func_tree->MainFunc_AmountOfArgs-1] = parser->curr_tokens->token_value;

            move_token_pointer(parser, Token_id);
            if(parser->curr_tokens->TokenType == Token_Colon)
                move_token_pointer(parser, Token_Colon);
            if(parser->curr_tokens->TokenType == Token_Comma)
                move_token_pointer(parser, Token_Comma);
            
            // Checking the type
            switch(parser->curr_tokens->TokenType) {
                case Token_Int: {
                    main_func_tree->MainFunc_argTypes = realloc(
                        main_func_tree->MainFunc_argTypes,
                        (main_func_tree->MainFunc_AmountOfArgs+1)*sizeof(main_func_tree->MainFunc_argTypes)
                    );
                    main_func_tree->MainFunc_argTypes[main_func_tree->MainFunc_AmountOfArgs-1] = parser->curr_tokens->token_value;
                    move_token_pointer(parser, Token_Int);
                    break;
                }
                case Token_Array: {
                    main_func_tree->MainFunc_argTypes = realloc(
                        main_func_tree->MainFunc_argTypes,
                        (main_func_tree->MainFunc_AmountOfArgs+2)*sizeof(main_func_tree->MainFunc_argTypes)
                    );

                    main_func_tree->MainFunc_argTypes[main_func_tree->MainFunc_AmountOfArgs-1] = parser->curr_tokens->token_value;

                    move_token_pointer(parser, Token_Array);

                    if(parser->curr_tokens->TokenType != Token_LT) {
                        fprintf(stderr,"\nMissing item types for Array.\n");
                        exit(EXIT_FAILURE);
                    }

                    move_token_pointer(parser, Token_LT);

                    switch(parser->curr_tokens->TokenType) {
                        case Token_Int: move_token_pointer(parser, Token_Int);break;
                        case Token_String: move_token_pointer(parser, Token_String); break;
                        default: {
                            fprintf(stderr,"\nUncaught Array type `%s`.\n\n");
                            exit(EXIT_FAILURE);
                        }
                    }

                    if(parser->curr_tokens->TokenType != Token_GT) {
                        fprintf(stderr,"\nMissing closing `>` for Array type declaration\n");
                        exit(EXIT_FAILURE);
                    }
                    move_token_pointer(parser, Token_GT);
                    break;
                }
                default: {
                    fprintf(stderr,"\nUncaught argument type `%s`(%d)\n",parser->curr_tokens->token_value, parser->curr_tokens->TokenType);
                    exit(EXIT_FAILURE);
                }
            }
            if(parser->curr_tokens->TokenType == Token_Comma)
                move_token_pointer(parser, Token_Comma);
            if(parser->curr_tokens->TokenType == Token_RP)
                move_token_pointer(parser, Token_RP);
        }
    }

    return (void*)0;
}

static SyntaxTree_* parse_function(Parser_* parser) {
    if(parser->curr_tokens->TokenType == Token_func_keyword) {
        move_token_pointer(parser,Token_func_keyword);

        if(parser->curr_tokens->TokenType == Token_id) {
            
            if(strcmp(parser->curr_tokens->token_value, "main")==0) {
                return parse_main_function(parser);
            }
            exit(EXIT_SUCCESS);
        }
    }

    return (void*)0;
}

static SyntaxTree_* parse_token(Parser_* parser) {
    switch(parser->curr_tokens->TokenType) {
        case Token_func_keyword: parse_function(parser);
    }

    return (void*)0;
}

static SyntaxTree_* start_parser(Parser_* parser) {
    SyntaxTree_* def_tree = parse_token(parser);

    while(parser->curr_tokens->TokenType == Token_semicolon) {
        move_token_pointer(parser, Token_semicolon);

        SyntaxTree_* tree = parse_token(parser);
    }

    return (void*)0;
}

SyntaxTree_* parse_bite(Parser_* parser) {
    return start_parser(parser);
}