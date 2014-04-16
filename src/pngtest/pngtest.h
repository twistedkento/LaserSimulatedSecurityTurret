#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#define ERROR 1
#define DEBUG 1

#include <stdio.h>
#include <png.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

typedef char* buffer;
extern int file_to_buffer(buffer*, char*);
extern int is_png(FILE*);
extern png_bytep* file_to_bytes(char*);
extern void _abort(char*);
