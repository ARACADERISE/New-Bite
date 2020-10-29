#ifndef read_file_
#define read_file_
#include <stdio.h>
#include <stdlib.h>

typedef struct FileBuffer {
    size_t file_size;
    long buffer_size;
} FileBuffer_;

char* read_file(char* filename);
char* file_check_extension(char* file_name, char* extension);

#endif