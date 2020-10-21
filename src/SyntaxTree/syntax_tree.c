#include "syntax_tree.h"

SyntaxTree_* init_syntax_tree(type) {
    SyntaxTree_* syntax_tree = calloc(1,sizeof(*syntax_tree));

    syntax_tree->BranchType = type;

    /* Branch_MainFunction */
    syntax_tree->function_name = (void*)0; // should be main
    syntax_tree->MainFunc_AmountOfArgs = 0; // no more than 2
    syntax_tree->MainFunc_args = (void*)0; // the argument names
    syntax_tree->MainFunc_argTypes = (void*)0; // the argument types
    syntax_tree->MainFuncReturnType = (void*)0;
    syntax_tree->integer_returned = 0; // execution succeeded if the return method was reached

    /* Branch_Function_Declaration */
    syntax_tree->func_name = (void*)0;
    syntax_tree->Funcargs = 0;
    syntax_tree->FunctionArgs = (void*)0;
    syntax_tree->FunctionArgTypes = (void*)0;

    /* Branch_Return_Keyword */
    syntax_tree->value_to_return = (void*)0; // this may change.

    /* Branch_String_Keyword */
    syntax_tree->string_variable_value = (void*)0;

    /* Branch_Int_Keyword */
    syntax_tree->int_varibale_value = 0; // this will be default values of integers

    /* Branch_Array_Keword */
    syntax_tree->array_type = (void*)0;
    syntax_tree->items = (void*)0; // this will be dependable upon the array_type
    syntax_tree->array_length = 0;

    return syntax_tree;
}