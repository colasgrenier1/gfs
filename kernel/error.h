#ifndef __ERROR
#define __ERROR

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char * message;
} error;

///Write an error message
error * error_new(char * fmt, ...);

///Write an error message to file.
void error_write(FILE * file, error * err);

///Display an error message to stderr
void error_display(error * err);

///Free an error message.
void error_free(error * err);

#endif
