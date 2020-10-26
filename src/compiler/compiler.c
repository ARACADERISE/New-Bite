#include "compiler.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//const char* DOT_GLOBAL = ".global";
//const char* MOVL = "movl";
//const char* RET = "ret";

// EAX REGISTER
//const char* EAX = "%%eax";

void CompileMainFunction(int return_value, int amount_of_variables, SyntaxTree_* tree) {

    FILE* file = fopen("cmpler.s","w");
    if(file){
        char* to_write = malloc(sizeof(*to_write));
        char* variables = malloc(sizeof(*variables));

        fprintf(file,"section .text\n\tglobal _start\n\t%%define system_call int 0x80");
        fprintf(file,"\n_start:\n");

        if(amount_of_variables > 0) 
        {
            char variables[amount_of_variables];
            for(int i = 0; i < amount_of_variables; i++)
            {
                //fprintf(file,"\n\tpush ebp\n\tmov ebp, esp\n\tmov ebp, esp\n\tpop ebp\n");
                fprintf(file,"\n\tmov ecx, %s\n",tree->main_function_variable_names[i]);

                sprintf(&variables[i],"%s",tree->main_function_variable_names[i]);

                fprintf(file,"\tpush ecx\n\tpush ebp\n\tmov ebp, ecx\n");
            }
            fprintf(file,"\tsystem_call\n");
        }
        fprintf(file,"\n\tmov eax, 1\n\tmov ebx, %d\n\tsystem_call\n",return_value);

        if(amount_of_variables > 0 )
        {
            fprintf(file,"\nsection .data");
            for(int i = 0; i < amount_of_variables; i++)
            {
                if(strcmp(tree->main_function_variable_types[i],"Int")==0)
                    fprintf(file,"\n\t%s equ %d",tree->main_function_variable_names[i], atoi(tree->main_function_variable_values[i]));
            }
        }
        
        fclose(file);
    }
    else
    {
        fprintf(stderr,"\nThe cmpler.s file was deleted.\n");
        exit(EXIT_FAILURE);
    }
}