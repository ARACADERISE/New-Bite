#include "parser.h"
#include <stdio.h>

Parser_* init_parser(lexer_* lexer) {
    Parser_* parser = calloc(1,sizeof(*parser));

    parser->lexer = lexer;
    parser->curr_tokens = get_next_token(parser->lexer);
    parser->prev_tokens = parser->curr_tokens;

    parser->StdLib = calloc(1,sizeof(*parser->StdLib));
    parser->StdLib->curr_std_lib = calloc(1,sizeof(*parser->StdLib->curr_std_lib));

    return parser;
}

static Parser_* move_token_pointer(Parser_* parser, int current_token) {
    if(parser->curr_tokens->TokenType == current_token) {
        parser->prev_tokens = parser->curr_tokens;
        parser->curr_tokens = get_next_token(parser->lexer);

        return parser;
    } else {
        fprintf(stderr,"\nError on line %d\n\tUnexpected value `%s`\n\t(Matched against token `%d`, when expecting `%d`)\n\n",parser->lexer->line,parser->curr_tokens->token_value,parser->curr_tokens->TokenType,current_token);
        exit(EXIT_FAILURE);
    }
}

static void parse_main_function_body(Parser_* parser, SyntaxTree_* tree, SyntaxTree_* all) {
    redo:
    switch(parser->curr_tokens->TokenType) {
        case Token_Make: { // variable declaration
            move_token_pointer(parser,Token_Make);
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
                fprintf(stderr,"\nVariable Error:\n\tLine: %d\n\tErr: Unrecognized variable declaration\n\tVariable: `%s`\n",parser->lexer->line,tree->main_function_variable_names[tree->amount_of_variables_-1]);
                tree->errors = 0;
                Exit();
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
                    tree->errors = 0;
                    Exit();
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

            for(int i = 0; i < tree->amount_of_variables_; i++)
            {
                if(strcmp(parser->curr_tokens->token_value,tree->main_function_variable_names[i])==0)
                {
                    tree->main_function_variable_values[tree->amount_of_variables_-1] = tree->main_function_variable_values[i];
                    break;
                }

                if(i == tree->amount_of_variables_-1)
                    tree->main_function_variable_values[tree->amount_of_variables_-1] = parser->curr_tokens->token_value;
            }
            //tree->main_function_variable_values[tree->amount_of_variables_-1] = parser->curr_tokens->token_value;

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
            tree->errors = 0;
            Exit();
        }
        case Token_return: {
            if(strcmp(tree->MainFuncReturnType,"Void")==0)
            {
                fprintf(stdout,"\n\033[3;31mError:\n\tEncountered return statement in Void function.\n\tLine: %d\n",parser->lexer->line);
                tree->errors = 0;
                Exit();
            }
            move_token_pointer(parser, Token_return);
            for(int i = 0; i < strlen(parser->curr_tokens->token_value); i++) {
                if(isdigit(parser->curr_tokens->token_value[i])) {
                    tree->main_function_return_val = atoi(&parser->curr_tokens->token_value[i]);
                    break;
                }

                if(i == strlen(parser->curr_tokens->token_value-1)) {
                    fprintf(stderr,"\nExpected a integer to be returned, got `%s`\n\tLine: %d\n", parser->curr_tokens->token_value,parser->lexer->line);
                    tree->errors = 0;
                    Exit();
                }
            }
            move_token_pointer(parser, Token_id); 
            move_token_pointer(parser,Token_semicolon);
            break;
        }
        default: {
            break;
        }
    }
}

static SyntaxTree_* parse_main_function(Parser_* parser, SyntaxTree_* all) {
    move_token_pointer(parser,Token_id); // "main"

    if(parser->curr_tokens->TokenType == Token_semicolon) {
        fprintf(stderr,"\nError on line %d:\n\tCannot pre-define the main function\n", parser->lexer->line);
        exit(EXIT_FAILURE);
    }

    move_token_pointer(parser, Token_LP);

    SyntaxTree_* main_func_tree = init_syntax_tree(Branch_MainFunction);
    
    if(parser->curr_tokens->TokenType == Token_id) {

        while(parser->curr_tokens->TokenType == Token_id) {

            if(main_func_tree->MainFunc_AmountOfArgs > 2) {
                fprintf(stderr,"\nToo many arguments for the main functions.\n\n");
                exit(EXIT_FAILURE);
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
    } else {
        main_func_tree->MainFuncReturnType = "Int";
    }

    if(parser->curr_tokens->TokenType != Token_LC) {
        fprintf(stderr,"\nError: Missing `{` for main function\n");
        exit(EXIT_FAILURE);
    }
    move_token_pointer(parser, Token_LC);

    parse_main_function_body(parser, main_func_tree,all);
    parser->curr_tokens->main_was_found = 0;

    return main_func_tree;
}

static SyntaxTree_* parse_function(Parser_* parser,SyntaxTree_* all) {
    if(parser->curr_tokens->TokenType == Token_func_keyword) {
        move_token_pointer(parser,Token_func_keyword);

        if(parser->curr_tokens->TokenType == Token_id) {
            
            if(strcmp(parser->curr_tokens->token_value, "main")==0) {
                return parse_main_function(parser,all);
            }
            
            if(strcmp(parser->StdLib->curr_std_lib,"std_io.b")==0)
            {
                SyntaxTree_* tree = init_syntax_tree(Branch_STD_LIB);
                tree->l_of_std_funcs++;
                tree->standard_functions_found = realloc(
                    tree->standard_functions_found,
                    (tree->l_of_std_funcs+1)*sizeof(*tree->standard_functions_found)
                );
                tree->standard_functions_found[tree->l_of_std_funcs-1] = parser->curr_tokens->token_value;
                move_token_pointer(parser,Token_id);

                move_token_pointer(parser,Token_LP);
                move_token_pointer(parser,Token_RP);

                if(parser->curr_tokens->TokenType == Token_Colon)
                {
                    move_token_pointer(parser,Token_Colon);

                    switch(parser->curr_tokens->TokenType)
                    {
                        case Token_Void: {
                            tree->standard_function_return_types = realloc(
                                tree->standard_function_return_types,
                                (tree->l_of_std_funcs+1)*sizeof(*tree->standard_function_return_types)
                            );
                            tree->standard_function_return_types[tree->l_of_std_funcs-1] = "Void";
                            move_token_pointer(parser,Token_Void);
                            break;
                        }
                        case Token_Int: {
                            tree->standard_function_return_types = realloc(
                                tree->standard_function_return_types,
                                (tree->l_of_std_funcs+1)*sizeof(*tree->standard_function_return_types)
                            );
                            tree->standard_function_return_types[tree->l_of_std_funcs-1] = "Int";
                            move_token_pointer(parser,Token_Int);
                            break;
                        }
                        default: {
                            break; // there should not be an error with standard libs
                        }
                    }
                }
                move_token_pointer(parser,Token_LC);

                switch(parser->curr_tokens->TokenType)
                {
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
                                Exit();
                            }
                            
                            REDO:
                            if(!(tree->main_function_variable_names)) {
                                tree-> n_items_to_print++;
                                tree->things_to_print = realloc(
                                    tree->things_to_print,
                                    (tree-> n_items_to_print+1)*sizeof(*tree->things_to_print)
                                );

                                if(isdigit(parser->curr_tokens->token_value[0]))
                                {
                                    tree->item_types = realloc(
                                        tree->item_types,
                                        (tree-> n_items_to_print+1)*sizeof(*tree->item_types)
                                    );
                                    tree->item_types[tree-> n_items_to_print-1] = "Int";
                                }

                                tree->things_to_print[tree-> n_items_to_print-1] = parser->curr_tokens->token_value;

                                move_token_pointer(parser,Token_id);
                            }
                            if(tree->main_function_variable_names)
                            {
                                int index = -1; // zero by default
                                for(int i = 0; i < tree->amount_of_variables_; i++)
                                {
                                    if(strcmp(tree->main_function_variable_names[i],parser->curr_tokens->token_value)==0)
                                    {
                                        index = i;
                                        break;
                                    }

                                    if(i == tree->amount_of_variables_-1)
                                    {
                                        if(isdigit(parser->curr_tokens->token_value[0])) {
                                            break;
                                        }
                                        fprintf(stderr,"\nThe variable `%s` does not exist.\n",parser->curr_tokens->token_value);
                                        Exit();
                                    }
                                }

                                if(index > -1){
                                    if(strcmp(tree->main_function_variable_types[index],"Int")==0)
                                    {
                                        tree-> n_items_to_print++;
                                        tree->things_to_print = realloc(
                                            tree->things_to_print,
                                            (tree-> n_items_to_print+1)*sizeof(*tree->things_to_print)
                                        );

                                        tree->things_to_print[tree-> n_items_to_print-1] = tree->main_function_variable_names[index];
                                    }
                                } else {
                                    tree-> n_items_to_print++;
                                    tree->things_to_print = realloc(
                                        tree->things_to_print,
                                        (tree-> n_items_to_print+1)*sizeof(*tree->things_to_print)
                                    );

                                    if(isdigit(parser->curr_tokens->token_value[0]))
                                    {
                                        tree->item_types = realloc(
                                            tree->item_types,
                                            (tree-> n_items_to_print+1)*sizeof(*tree->item_types)
                                        );
                                        tree->item_types[tree-> n_items_to_print-1] = "Int";
                                    }

                                    tree->things_to_print[tree-> n_items_to_print-1] = parser->curr_tokens->token_value;
                                }

                                move_token_pointer(parser, Token_id);

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
                                    tree->errors = 0;
                                    Exit();
                                    //exit(EXIT_FAILURE);
                                }

                                move_token_pointer(parser, Token_semicolon);
                            }
                        }

                        move_token_pointer(parser,Token_RP);
                        move_token_pointer(parser, Token_semicolon);

                        if(parser->curr_tokens->TokenType == Token_RC)
                        {
                            //printf("\nCompiled successfuly\n");
                            break;
                        }
                        //exit(EXIT_SUCCESS);
                    }
                }
                move_token_pointer(parser,Token_RC);
                return tree;
            }
        }
    
    }
    return (void*)0;
}

SyntaxTree_* parse_use_method(Parser_* parser)
{
    move_token_pointer(parser,Token_Use);

    if(parser->curr_tokens->TokenType == Token_SingleQuotes)
    {
        move_token_pointer(parser,Token_SingleQuotes);
        
        SyntaxTree_* tree = init_syntax_tree(Branch_Use_Keyword);

        tree->l_of_imports++;
        tree->files_to_import = realloc(
            tree->files_to_import,
            (tree->l_of_imports+1)*sizeof(*tree->files_to_import)
        );

        tree->files_to_import[tree->l_of_imports-1] = parser->curr_tokens->token_value;
        strcat(tree->files_to_import[tree->l_of_imports-1],".b");
        move_token_pointer(parser, Token_id);

        if(parser->curr_tokens->TokenType == Token_SingleQuotes) 
        {
            move_token_pointer(parser,Token_SingleQuotes);

            if(strcmp(tree->files_to_import[tree->l_of_imports-1],"std_io.b")==0)
            {
                char* filename = malloc(sizeof(*filename)*4);
                sprintf(filename,"standard/%s",tree->files_to_import[tree->l_of_imports-1]);
                
                char* code = read_file(filename);
                lexer_* lexer = init_lexer(code);
                Parser_* parser_ = init_parser(lexer);
                parser_->StdLib->curr_std_lib = realloc(
                    parser_->StdLib->curr_std_lib,
                    (strlen(tree->files_to_import[tree->l_of_imports-1]+1)*sizeof(*parser_->StdLib->curr_std_lib))
                );
                parser_->StdLib->curr_std_lib = tree->files_to_import[tree->l_of_imports-1];
                parse_bite(parser_);
            }
            free(parser->StdLib->curr_std_lib);
            return tree;
        }
        fprintf(stderr,"\nMissing `'` on line %d\n",parser->lexer->line);
        Exit();
    }
    fprintf(stderr,"\nSyntax error on line %d\n\tExpected `'`, got %s\n",parser->lexer->line,parser->curr_tokens->token_value);
    exit(EXIT_FAILURE);
}

static SyntaxTree_* parse_token(Parser_* parser, SyntaxTree_* current_tree, SyntaxTree_* all) {
    switch(parser->curr_tokens->TokenType) {
        case Token_func_keyword:return parse_function(parser,all);
        case Token_Use: return parse_use_method(parser);
        case Token_id: {
            fprintf(stderr,"\nSyntaxError:\n\tLine: %d\n\tThe value `%s` was not found inside a function, struct or enum.\n",parser->lexer->line,parser->curr_tokens->token_value);
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
    def_tree = parse_token(parser,def_tree,def_tree);

    def_tree->combined[0] = def_tree;
    def_tree->length++;

    while(parser->curr_tokens->TokenType == Token_semicolon) {
        move_token_pointer(parser, Token_semicolon);

        SyntaxTree_* tree = parse_token(parser,tree,def_tree);

        if(tree)
        {
            def_tree->length++;
            def_tree->combined = realloc(
                def_tree->combined,
                (def_tree->length+1)*sizeof(*def_tree->combined)
            );
            def_tree->combined[def_tree->length-1] = tree;
        }
    }

    return def_tree;
}

SyntaxTree_* parse_bite(Parser_* parser) {
    return start_parser(parser);
}