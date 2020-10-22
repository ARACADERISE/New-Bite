#ifndef ST
#define ST
#include <stdlib.h>

typedef struct SyntaxTree {
    enum {
        Branch_MainFunction,
        Branch_Function_Declaration,
        Branch_Return_Keyword,
        Branch_String_Keyword,
        Branch_Int_Keyword,
        Branch_Array_Keword,
    } BranchType;

    /* Branch_MainFunction */
    char* function_name; // should be main
    int MainFunc_AmountOfArgs; // no more than 2
    char** MainFunc_args; // the argument names
    char** MainFunc_argTypes; // the argument types
    char* MainFuncReturnType;
    int integer_returned; // the main function is int only, and ONLY EVER int
    char** main_function_variable_names;
    void** main_function_variable_values;
    int amount_of_variables_;

    /* Branch_Function_Declaration */
    char* func_name;
    int Funcargs;
    char** FunctionArgs;
    char** FunctionArgTypes;
    char* FunctionReturnType;
    char** function_variable_names;
    void** function_variable_values;
    int _amount_of_variables;

    /* Branch_Return_Keyword */
    char* value_to_return; // this may change.

    /* Branch_String_Keyword */
    char* string_variable_value;

    /* Branch_Int_Keyword */
    int int_varibale_value;

    /* Branch_Array_Keword */
    char* array_type;
    void** items; // this will be dependable upon the array_type
    int array_length; // this increments as the parser finds more items within the array

} SyntaxTree_;

SyntaxTree_* init_syntax_tree(int type);

#endif