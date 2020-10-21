#ifndef read_file_
#define read_file_
#include <stdio.h>
#include <stdlib.h>

typedef struct FileBuffer {
    size_t file_size;
    long buffer_size;
} FileBuffer_;

char* read_file(char* filename);

#endif