#include "compiler.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const char* DOT_GLOBAL = ".global";
const char* MOVL = "movl";
const char* RET = "ret";

// EAX REGISTER
const char* EAX = "%%eax";

void CompileMainFunction(int return_value, int amount_of_variables) {

    FILE* file = fopen("cmpler.s","w");
    if(file){
        char* to_write = malloc(sizeof(*to_write));
        char* variables = malloc(sizeof(*variables));

        fprintf(file,".globl _start\n");
        fprintf(file,"\n_start:\n\tmovl $%d, %%ebp\n",return_value);
        fprintf(file,"\tmovl $1, %%eax\n");

        if(amount_of_variables > 0) {
            for(int i = 0; i < amount_of_variables; i++)
            {
                fprintf(file,"\n\tpush %%ebp\n\tmovl %%esp, %%ebp\n\tmovl %%ebp, %%esp\n\tpop %%ebp\n");
            }
        }

        fprintf(file,"\n\tint $0x80\n\tret\n");
        
        fclose(file);
    }
    else
    {
        fprintf(stderr,"\nThe cmpler.s file was deleted.\n");
        exit(EXIT_FAILURE);
    }
}