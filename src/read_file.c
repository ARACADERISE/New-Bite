#include "read_file.h"
#include "helpers.h"
#include <string.h>

FileBuffer_* assign(size_t size) {
    FileBuffer_* buffer = calloc(1,sizeof(*buffer));

    buffer->file_size = size;
    buffer->buffer_size = size*8;

    return buffer;
}

char* convert_to_string(char char_) {
    char* str = calloc(2,sizeof(*str));

    str[0] = char_;
    str[1] = '\0';
    return str;
}

char* file_check_extension(char* file_name, char* extension) {

    int index = 0;

    char* curr_extension = calloc(1,sizeof(char));

    for(; index < strlen(file_name); index++) {
        if(file_name[index] == '.') {
            do {

                char* new_val = convert_to_string(file_name[index]);

                curr_extension = realloc(
                    curr_extension,
                    (strlen(curr_extension)+2)*sizeof(char*)
                );
                strcat(curr_extension,new_val);

                index++;
                
            } while(file_name[index] != '\0');
        }
    }

    if(!(strcmp(curr_extension,extension)==0))
    {
        //strcat(file_name,extension);
        fprintf(stderr,"\nBite compiler cannot read a file with extensions `%s`\n",curr_extension);
        exit(EXIT_FAILURE);
    }

    return file_name;
}

char* read_file(char* filename) {

    file_check_extension(filename,".b");
    FILE* source_code = fopen(filename,"rb");
    char* code = calloc(1,sizeof(char));
    long size;

    if(source_code) {
        fseek(source_code,0,SEEK_END);
        size = ftell(source_code);
        FileBuffer_* fb = assign(size);
        fseek(source_code,0,SEEK_SET);

        if(CheckStrict(fb->file_size, size) == 0) {
            code = realloc(
                code,
                size*sizeof(char*)
            );

            if(code) {
                fread(code,1,fb->buffer_size,source_code);
            } else {
                fprintf(stderr,"\nError on allocating heap memory for interpretation.\n\tTry Again\n");
                exit(HeapAllocationgError);
            }

            return code;
        } else {
            fprintf(stderr,"\nFile buffer size did not match with it's told size.\n\tMemory Error\n");
            exit(FileMemoryError);
        }
    } else {
        fprintf(stderr,"\nNo such file or directory\n");
        exit(FileOrDirectoryNotFound);
    }
}