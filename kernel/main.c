/*
 *
 *  USAGE:
 *		./gfs {fmt|format}=<format>
 *            [in[put]=<input file>] [out[put]=<output file>]
 *
 */

#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>

#include "error.h"
#include "runtime.h"

int main(int argc, char ** arg) {
	//Input and output streams
	FILE * in;
	FILE * out;
	//Runtime
	runtime * rt;

	//Error value
	error * err;

	//For now
	in = stdin;
	out = stdout;

	//We initialize a runtime
	rt = runtime_new(stdout);
	//We load modules in the GFS_EXTENSIONS directory
	err = runtime_loaddir(rt, getenv("GFSEXTENSIONS"));



}
