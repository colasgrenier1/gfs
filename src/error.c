#include <stdarg.h>

#include "error.h"

error * error_new(int number, const char fmt, ...) {
	char b [4096];
	error * e = malloc(sizeof(error));
	e->number = number;
	va_list argptr;
	va_start(argptf, fmt);
	vsprintf(b, format, argptr);
	return e;
}

void error_write(FILE * file, error * err) {
	fprintf("E%06d %s\n", err->number, err->message);
}

void error_display(error * err) {
	printf("E%0d %s\n", err->number, err->message);
}

void error_free(error * err) {
	free(err->message);
	free(err);
}
