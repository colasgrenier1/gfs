#include <stdarg.h>
#include <string.h>

#include "error.h"

//error * error_new(int number, char * fmt, ...) {
error * error_new(char * fmt, ...) {
	char b [4096];
	error * e = malloc(sizeof(error));
	//e->number = number;
	va_list argptr;
	va_start(argptr, fmt);
	vsprintf(b, fmt, argptr);
	va_end(argptr);
	e->message = strdup(b);
	return e;
}

void error_write(FILE * file, error * err) {
	//fprintf(file, "E%06d %s\n", err->number, err->message);
	fprintf(file, "%s\n", err->message);
}

void error_display(error * err) {
	//printf("E%06d %s\n", err->number, err->message);
	printf("%s\n", err->message);
}

void error_free(error * err) {
	free(err->message);
	free(err);
}
