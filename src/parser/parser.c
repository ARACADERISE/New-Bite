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
        fprintf(stderr,"\nError on line %d\n\tUnexpected value `%s`\n\t(Matched against token `%d`, when expecting `%d`)",parser->lexer->line,parser->curr_tokens->token_value,parser->curr_tokens->TokenType,current_token);
        exit(1);
    }
}

static void parse_main_function_body(Parser_* parser, SyntaxTree_* tree) {
    redo:
    switch(parser->curr_tokens->TokenType) {
        case Token_id: { // variable declaration
            tree->amount_of_variables_++;
            if(!(tree->main_function_variable_names))
                tree->main_function_variable_names = calloc(
                    tree->amount_of_variables_+1,
                    sizeof(tree->main_function_variable_names)
                );
            else
            {
                tree->main_function_variable_names = realloc(
                    tree->main_function_variable_names,
                    (tree->amount_of_variables_+1)*sizeof(tree->main_function_variable_names)
                );
            }
            tree->main_function_variable_names[tree->amount_of_variables_-1] = parser->curr_tokens->token_value;
            move_token_pointer(parser, Token_id);

            if(parser->curr_tokens->TokenType != Token_Colon) {
                fprintf(stderr,"\nVariable Error:\n\tLine: %d\n\tErr: Unrecognized variable declaration\n\tVariable: `%s`\n",parser->lexer->line-1,tree->main_function_variable_names[tree->amount_of_variables_-1]);
                exit(EXIT_FAILURE);
            }
            move_token_pointer(parser, Token_Colon);
            if(!(tree->main_function_variable_types))
                tree->main_function_variable_types = calloc(
                    tree->amount_of_variables_,
                    sizeof(*tree->main_function_variable_types)
                );
            else
                tree->main_function_variable_types = realloc(
                    tree->main_function_variable_types,
                    (tree->amount_of_variables_+1)*sizeof(*tree->main_function_variable_types)
                );
            switch(parser->curr_tokens->TokenType) {
                case Token_Int: {
                    tree->main_function_variable_types[tree->amount_of_variables_-1] = parser->curr_tokens->token_value;
                    move_token_pointer(parser, Token_Int);
                    break;
                }
                case Token_String: {
                    tree->main_function_variable_types[tree->amount_of_variables_-1] = parser->curr_tokens->token_value;
                    move_token_pointer(parser, Token_String);
                    break;
                }
                default: {
                    fprintf(stderr,"\nUncaught type for variable declaration on line %d\n\n",parser->lexer->line);
                    exit(EXIT_FAILURE);
                }
            }
            move_token_pointer(parser, Token_Equals);
            if(!(tree->main_function_variable_values))
                tree->main_function_variable_values = calloc(
                    tree->amount_of_variables_,
                    sizeof(*tree->main_function_variable_values)
                );
            else
                tree->main_function_variable_values = realloc(
                    tree->main_function_variable_values,
                    (tree->amount_of_variables_+1)*sizeof(*tree->main_function_variable_values)
                );
            tree->main_function_variable_values[tree->amount_of_variables_-1] = parser->curr_tokens->token_value;
            move_token_pointer(parser, Token_id);
            move_token_pointer(parser, Token_semicolon);
            
            if(parser->curr_tokens->TokenType == Token_RC)
            {
                //printf("\nCompiled successfuly!\n");
                break;
            }
            goto redo;
        }
        case Token_Int: {
            fprintf(stderr,"\nUncaught reference to `Int` on line %d\n\n",parser->lexer->line);
            exit(EXIT_FAILURE);
        }
        case Token_Print_Function: {
            /*
                THIS IS JUST TESTING!
                THIS WILL BE REMOVED!
            */
            move_token_pointer(parser, Token_Print_Function);

            if(parser->curr_tokens->TokenType == Token_LP)
            {
                // one type of syntax
                move_token_pointer(parser, Token_LP);

                if(parser->curr_tokens->TokenType != Token_id)
                {
                    fprintf(stderr,"\nMissing arguemtns for built-in Print statement.\n");
                    exit(EXIT_FAILURE);
                }
                
                REDO:
                if(tree->main_function_variable_names)
                {
                    int index = 0; // zero by default
                    for(int i = 0; i < tree->amount_of_variables_; i++)
                    {
                        if(strcmp(tree->main_function_variable_names[i],parser->curr_tokens->token_value)==0)
                        {
                            index = i;
                            break;
                        }

                        if(i == tree->amount_of_variables_-1)
                        {
                            if(isdigit(parser->curr_tokens->token_value[i])) {
                                //printf("OHH");
                                break;
                            }
                            fprintf(stderr,"\nThe variable `%s` does not exist.\n",parser->curr_tokens->token_value);
                            exit(EXIT_FAILURE);
                        }
                    }

                    move_token_pointer(parser, Token_id);
                    if(strcmp(tree->main_function_variable_types[index],"Int")==0)
                    {
                        int a = atoi(tree->main_function_variable_values[index]);
                        printf("%d",a);
                    }

                    if(parser->curr_tokens->TokenType == Token_Comma)
                    {
                        move_token_pointer(parser, Token_Comma);
                        goto REDO;
                    }

                    if(parser->curr_tokens->TokenType == Token_RP)
                        move_token_pointer(parser, Token_RP);
                    else
                    {
                        fprintf(stderr,"\n\033[3;31mError:\n\tMissing `)` for Print function\n\tLine: %d\n",parser->lexer->line);
                        exit(EXIT_FAILURE);
                    }

                    move_token_pointer(parser, Token_semicolon);
                }
            }
            else
            {
                // second type of syntax
                printf("%s",parser->curr_tokens->token_value);
            }

            if(parser->curr_tokens->TokenType == Token_RC)
            {
                //printf("\nCompiled successfuly\n");
                break;
            }
            goto redo;
            //exit(EXIT_SUCCESS);
        }
        case Token_return: {
            if(strcmp(tree->MainFuncReturnType,"Void")==0)
            {
                fprintf(stdout,"\n\033[3;31mError:\n\tEncountered return statement in Void function.\n\tLine: %d\n",parser->lexer->line);
                exit(EXIT_FAILURE);
            }
            move_token_pointer(parser, Token_return);
            for(int i = 0; i < strlen(parser->curr_tokens->token_value); i++) {
                if(isdigit(parser->curr_tokens->token_value[i])) {
                    tree->integer_returned = atoi(&parser->curr_tokens->token_value[i]);
                    break;
                }

                if(i == strlen(parser->curr_tokens->token_value-1)) {
                    fprintf(stderr,"\nExpected a integer to be returned, got `%s`\n\tLine: %d\n", parser->curr_tokens->token_value,parser->lexer->line);
                    exit(EXIT_FAILURE);
                }
            }
            move_token_pointer(parser, Token_id); 
            move_token_pointer(parser,Token_semicolon);
            // This will most likely change!
            // It is just a comfort thing for now
            // ToDo: Have execution codes(0 and 1) be returned and the compiler pick them up
            if(tree->integer_returned == 0) {
                //fprintf(stdout,"\nCompiled successfuly!\n");
                //exit(EXIT_SUCCESS);
                //break;
            } else {
                //fprintf(stderr,"\n\033[3;31mError: Execution exited with exit status %d\n\n\033[0m",tree->integer_returned);
                //exit(EXIT_FAILURE);
                //break;
            }
            CompileMainFunction(tree->integer_returned, tree->amount_of_variables_, tree);
            break;
        }
        default: {
            break;
        }
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
                            fprintf(stderr,"\nUncaught Array type `%s`.\n\n", parser->curr_tokens->token_value);
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
    } else {
        move_token_pointer(parser, Token_RP);
    }
    
    if(parser->curr_tokens->TokenType == Token_Colon) {
        move_token_pointer(parser, Token_Colon);

        switch(parser->curr_tokens->TokenType) {
            case Token_Int: {
                main_func_tree->MainFuncReturnType = parser->curr_tokens->token_value;
                move_token_pointer(parser, Token_Int);
                break;
            }
            case Token_Void: {
                main_func_tree->MainFuncReturnType = parser->curr_tokens->token_value;
                move_token_pointer(parser, Token_Void);
                break;
            }
            default: {
                fprintf(stderr,"\nThe main function can only return an integer or void\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    if(parser->curr_tokens->TokenType != Token_LC) {
        fprintf(stderr,"\nError: Missing `{` for main function\n");
        exit(EXIT_FAILURE);
    }
    move_token_pointer(parser, Token_LC);

    parse_main_function_body(parser, main_func_tree);
    parser->curr_tokens->main_was_found = 0;

    return main_func_tree;
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

static SyntaxTree_* parse_token(Parser_* parser, SyntaxTree_* current_tree) {
    switch(parser->curr_tokens->TokenType) {
        case Token_func_keyword:return parse_function(parser);
        case Token_id: {
            fprintf(stderr,"\nSyntaxError:\n\tLine: %d\n\tThe value `%s` was not found inside a function, struct or enum.\n",parser->lexer->line-1,parser->curr_tokens->token_value);
            exit(EXIT_FAILURE);
        }
        default: {
            fprintf(stderr,"\nUncaught value on line %d\n\t`%s`\n",parser->lexer->line-1,parser->curr_tokens->token_value);
            exit(EXIT_FAILURE);
        }
    }

    return (void*)0;
}

static SyntaxTree_* start_parser(Parser_* parser) {
    SyntaxTree_* def_tree = calloc(1,sizeof(*def_tree));
    def_tree = parse_token(parser,def_tree);

    while(parser->curr_tokens->TokenType == Token_semicolon) {
        move_token_pointer(parser, Token_semicolon);

        SyntaxTree_* tree = parse_token(parser,tree);
    }

    return (void*)0;
}

SyntaxTree_* parse_bite(Parser_* parser) {
    return start_parser(parser);
}