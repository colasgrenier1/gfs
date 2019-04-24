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

	printf("starting\n");

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
	if (getenv("GFSEXTENSIONS")) {
		err = runtime_loaddir(rt, getenv("GFSEXTENSIONS"));
		if (err != NULL) {
			printf("runtime loaddirf ailed\n");
			exit(-1);
		}
	}

	//We load modules from the configuration file

	//TEMPORARY: read modules from /bin/builtin
	err = runtime_loaddir(rt, "/home/nicolas/gfs/bin/builtin");
	if (err != NULL) {
		printf("RUNTIMELOADDIR FAILED\n");
		error_display(err);
		exit(-1);
	}

	//We dump the runtime tables
	runtime_dump_tables(rt, stdout);
}
