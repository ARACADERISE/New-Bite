#include "read_file.h"
#include "helpers.h"

FileBuffer_* assign(size_t size) {
    FileBuffer_* buffer = calloc(1,sizeof(*buffer));

    buffer->file_size = size;
    buffer->buffer_size = size*8;

    return buffer;
}

char* read_file(char* filename) {
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