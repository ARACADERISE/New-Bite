#include "compiler.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void CompileMainFunction(FILE* file, int return_value, int amount_of_variables, SyntaxTree_* tree) {

    if(file){
        char* to_write = malloc(sizeof(*to_write));
        char* variables = malloc(sizeof(*variables));

        fprintf(file,"main:\n");
        //fprintf(file,"section .text\n\tglobal _start\n\t%%define system_call int 0x80\n\n\tfive: db 0xCD\n\tlen: equ $ - five");
        //fprintf(file,"\n_start:\n");

        if(amount_of_variables > 0) 
        {
            char variables[amount_of_variables];
            for(int i = 0; i < amount_of_variables; i++)
            {
                //fprintf(file,"\n\tpush ebp\n\tmov ebp, esp\n\tmov ebp, esp\n\tpop ebp\n");
                fprintf(file,"\n\tmov ecx, %s\n\tsystem_call\n",tree->main_function_variable_names[i]);

                sprintf(&variables[i],"%s",tree->main_function_variable_names[i]);
            }
        }
        if(tree-> n_items_to_print > 0)
        {
            fprintf(file,"\n\tcall print\n");
        }

        fprintf(file,"\n\tmov eax, 1\n\tmov ebx, %d\n\tsystem_call\n",tree->main_function_return_val);
    }
    else
    {
        fprintf(stderr,"\nThe cmpler.s file was deleted.\n");
        exit(EXIT_FAILURE);
    }
}

void evaluate_variables(FILE* file, SyntaxTree_* tree) {
/* Evaluating all variables and printing ideals*/
    if(tree->amount_of_variables_ > 0)
    {
        fprintf(file,"\nsection .rodata");
        fprintf(file,"\n\tmsg db \"\",0xa\n\tlen equ $ - msg\n");
        for(int i = 0; i < tree->amount_of_variables_; i++)
        {
            if(strcmp(tree->main_function_variable_types[i],"Int")==0)
                fprintf(file,"\n\t%s db '%d'",tree->main_function_variable_names[i], atoi(tree->main_function_variable_values[i]));
        }
    }
    if(tree->_amount_of_variables)
    {
        for(int i = 0; i < tree->_amount_of_variables; i++)
        {
            if(strcmp(tree->function_variable_types[i],"Int"))
                fprintf(file,"\n\t%s db '%d'",tree->function_variable_names[i], atoi(tree->function_variable_values[i]));
        }
    }
}
void evaluate_print_function(FILE* file, SyntaxTree_* tree)
{
    if(file)
    {

        fprintf(file,"\nprint:\n");
        for(int i = 0; i < tree-> n_items_to_print; i++)
        {
            char* tree_val = tree->things_to_print[i];
            if(isdigit(tree_val[0]))
            {
                fprintf(file,"\n\tmov eax, '%d'\n\tmov [var], eax\n\tmov ecx, var\n\tmov edx, %ld\n\tmov eax, 4\n\tmov ebx, 1\n\tsystem_call\n\n\t;LINE BREAK\n\tmov ecx, msg\n\tmov edx, len\n\tmov eax, 4\n\tmov ebx, 1\n\tsystem_call\n",atoi(tree_val),strlen(tree_val));
            }
            for(int x = 0; x < tree->amount_of_variables_; x++)
            {
                if(strcmp(tree->things_to_print[i],tree->main_function_variable_names[x])==0)
                {
                    fprintf(file,"\n\tmov eax, [%s]\n\tmov [var], eax\n\tmov ecx, var\n\tmov edx, %ld\n\tmov eax, 4\n\tmov ebx, 1\n\tsystem_call\n\n\t;LINE BREAK\n\tmov ecx, msg\n\tmov edx, len\n\tmov eax, 4\n\tmov ebx, 1\n\tsystem_call\n",tree->main_function_variable_names[x],strlen(tree->main_function_variable_values[x]));
                }
                else 
                {
                    if(x == tree->amount_of_variables_ - 1) break;
                }
            }
        }
        fprintf(file,"\n\tmov eax, 1\n\tmov ebx, 0\n\tsystem_call\n");

        fprintf(file,"\nsegment .bss\n\tvar resb 4");

        if(!(tree->main_function_variable_names))
        {
            /*fprintf(file,"\nsegment .rodata:\n");
            fprintf(file,"\n\tmsg db \"\", 0xa\n\tlen equ $ - msg\n");*/
        }
    }
}

void Exit()
{
    return exit(EXIT_FAILURE);
}

void COMPILE(SyntaxTree_* tree) {
    
    int main_found = -1;
    for(int i = 0; i < tree->length; i++)
    {
        if(tree->combined[i]->MainFuncReturnType && tree->errors == 1) // Int by default
        {
            FILE* file = fopen("cmpler.s","w");

            if(file)
            {

                /* This is where all of the functions will be globalized */
                fprintf(file,"section .text\n\tglobal _start\n\tglobal err\n\tglobal main\n\tglobal print\n\t%%define system_call int 0x80\n");

                fprintf(file,"\n;This will be used if there is a error captured during\n;compilation\nerr:\n\n\tmov eax, 1\n\tmov ebx, 1\n\tsystem_call\n");

                /* STARTUP OF THE MAIN FUNCTION */
                CompileMainFunction(file,tree->main_function_return_val, tree->combined[i]->amount_of_variables_, tree->combined[i]);

                fprintf(file,"\n\n_start:\n\n\tpush ebp\n\tmov ebp, esp\n\tsub esp, 16\n\tsystem_call\n\n\tcall main\n\tpop ebp\n\tmov esp, ebp\n\tsystem_call\n\n\tmov eax, 1\n\tmov ebx, 0\n\tsystem_call\n");

                //fprintf(file,"\n\tmov esp, ebp\n\tpop ebp\n\tsystem_call\n\n\tmov eax, 1\n\tmov ebx, %d\n\tsystem_call\n",tree->main_function_return_val);

                if(tree->combined[i]->n_items_to_print != 0)
                {
                    evaluate_print_function(file,tree->combined[i]);
                }

                /* Evaluating main function variables */
                if(tree->combined[i]->main_function_variable_names)
                    evaluate_variables(file,tree->combined[i]);

                fclose(file);
                main_found = 0;
            }
        } else {
            if(i == tree->length-1)
            {
                fprintf(stderr,"\nError: Main function was now encountered\n\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}