#ifndef helper
#define helper

// MACROS
#define CheckStrict(size, to) ( (((size|to)&to) == size) ? 0 : 1 )
// END MACROS

// Error Codes
#define FileOrDirectoryNotFound 1
#define FileMemoryError -1
#define HeapAllocationgError 2
// End Error Code

#endif