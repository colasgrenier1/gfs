#include <dirent.h>
#include <dlfcn.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"
#include "runtime.h"
#include "error.h"

/*******************************************************************************
 *                                                                             *
 *                         R U N T I M E   T A B L E S                         *
 *                                                                             *
 ******************************************************************************/

/*
 * EXTENSION TABLE
 */

 runtime_extension_table * runtime_extension_table_new() {
	 runtime_extension_table * p = malloc(sizeof(runtime_extension_table));
	 p->size = 0;
	 p->alloc = RUNTIME_EXTENSION_TABLE_DEFAULT_SIZE;
	 p->tbl = calloc(sizeof(char *), RUNTIME_EXTENSION_TABLE_DEFAULT_SIZE);
	 return p;
 }

 error * runtime_extension_table_add(runtime_extension_table * tbl, char * name) {
	 //If we do not have space we extend
	 if (tbl->size >= tbl->alloc) {
		 char ** old = tbl->tbl;
		 int tmps = tbl->size;
		 tbl->tbl = malloc(sizeof(char *)*(tbl->size+RUNTIME_EXTENSION_TABLE_INCREMENT));
		 memcpy(tbl->tbl, old, sizeof(char *)*tmps);
	 }
	 //We copy
	 tbl->tbl[tbl->size] = name;
	 tbl->size += 1;
	 return NULL;
 }

/*
 * ELEMENT TABLE
 */

runtime_element_table * runtime_element_table_new() {
	runtime_element_table * p = malloc(sizeof(runtime_element_table));
	p->size = 0;
	p->tbl = calloc(sizeof(runtime_element_table_elem), RUNTIME_ELEMENT_TABLE_DEFAULT_SIZE);
	p->alloc = RUNTIME_ELEMENT_TABLE_DEFAULT_SIZE;
	return p;
}

error * runtime_element_table_add(runtime_element_table * tbl, char * name, element_control_block * ecb) {
	//If we do not have space we add space
	if (tbl->size >= tbl->alloc) {
		//We readjust
		runtime_element_table_elem * tmp = tbl->tbl;
		int tmps = tbl->size;
		tbl->tbl = calloc(sizeof(runtime_element_table_elem), tbl->alloc + RUNTIME_ELEMENT_TABLE_INCREMENT);
		memcpy(tbl->tbl, tmp, tmps*sizeof(runtime_element_table_elem));
	}
	tbl->tbl[tbl->size].name = name;
	tbl->tbl[tbl->size].ecb = ecb;
	tbl->size += 1;
	return NULL;
}

element_control_block * runtime_element_table_get(runtime_element_table * tbl, char * name) {
	int i;
	for (i=0; i<tbl->size; i++) {
		if (strcmp(tbl->tbl[i].name, name)==0) {
			return tbl->tbl[i].ecb;
		}
	}
	return NULL;
}


/*******************************************************************************
 *                                                                             *
 *                               R U N T I M E                                 *
 *                                                                             *
 ******************************************************************************/

 runtime * runtime_new(FILE * log) {
	 //Allocate
	 runtime * rt = malloc(sizeof(runtime));
	 //Set the log file
	 rt->log = log;
	 runtime_log(rt, "ALLOCATING RUNTIME\n");
	 //Create the tables
	 rt->extensions  = runtime_extension_table_new();
	 rt->elements = runtime_element_table_new();
	 rt->commands = command_table_new();
 }

void runtime_log(runtime * rt, char * fmt, ...) {
	if (rt->log == NULL) {
		return;
	} else {
		va_list args;
		va_start(args, fmt);
		vfprintf(rt->log, fmt, args);
		va_end(args);
	}
}

error * runtime_load(runtime * rt, char * file) {

	//We start by logging
	runtime_log(rt, "RUNTIME LOADING FROM '%s'\n",file);

	//Index variables
	int idx;
	void * tmp;
	char * tmp1;
	char * tmp2;
	//We first try to load from the library
	void * ptr = dlopen(file, RTLD_NOW);
	if (ptr==NULL) {
		return error_new("COULD NOT LOAD PLUGIN \'%s\': %s", file, dlerror());
	}

	//We add the library to the runtime
	runtime_extension_table_add(rt->extensions, file);

	//We initialize the library
	error* (*initialize)(runtime * rt);
	initialize = (error* (*)(runtime * rt))dlsym(ptr, "initialize");
	if (initialize != NULL) {
		error * err = initialize(rt);
		if (err != NULL) {
			return err;
		}
	} else {
		return error_new("COULD NOT LOAD PLUGIN \'%s\' : %s", file, dlerror());
	}
	return NULL;
}

error * runtime_loaddir(runtime * rt, char * dir) {
	//We start by logging
	runtime_log(rt, "LOADING FROM DIRECTORY '%s'\n", dir);
	//Buffer to concatenate directory
	char b[4096];
	//Elements
	DIR * d;
	struct dirent * de;
	d = opendir(dir);
	if (d != NULL) {
		while ((de = readdir(d)) != NULL) {
			//if (strlen(de->d_name)>4 && strcmp(de->d_name+,".gfx")==
			int l = strlen(de->d_name);
			if (l>4 && strcmp((de->d_name+l-4),".gfx")==0) {
				//We make the total directory path
				strcpy(b, dir);
				strcat(b, "/");
				strcat(b, de->d_name);
				error * e = runtime_load(rt, b);
				if (e != NULL) {
					return e;
				}
			}
		}
	} else {
		return error_new("COULD NOT OPEN DIRECTORY '%s'", dir);
	}
}

/*******************************************************************************
 *                                                                             *
 *                  R U N T I M E   R E G I S T R A T I O N                    *
 *                                                                             *
 ******************************************************************************/

error * runtime_register_command(runtime * rt, char * name, command * cmd) {
	return command_table_add(rt->commands, name, cmd);
}

error * runtime_register_element(runtime * rt, char * name, element_control_block * ecb) {
	return runtime_element_table_add(rt->elements, name, ecb);
}

element_control_block * runtime_element_control_block_get(runtime * rt, char *name) {
	return runtime_element_table_get(rt->elements, name);
}

error * runtime_register_renderer(runtime * rt, char * elem, char * cmd, renderer * r) {
	//We first get the renderer

}

/*******************************************************************************
 *
 *                          E X E C U T I O N
 *
 ******************************************************************************/

error * render_element_children(element * elem, char * fmt, FILE * file) {
	return NULL;
}

/*******************************************************************************
 *                                                                             *
 *                                 D E B U G                                   *
 *                                                                             *
 ******************************************************************************/

void * runtime_dump_tables(runtime * rt, FILE * file) {
	int i;

	//Say what we are dumping
	fprintf(file, "DUMPING TABLES FOR RUNTIME %p\n", rt);

	//Dump the commands
	fprintf(file, "    DUMPING %d COMMAND(S)\n", rt->commands->size);
	for (i=0; i<rt->commands->size; i++) {
		fprintf(file, "        %-20s %p\n", rt->commands->tbl[i].name, rt->commands->tbl[i].cmd);
	}

	//Dump the elements
	fprintf(file, "    DUMPING %d ELEMENT(S)\n", rt->elements->size);
	for (i=0; i<rt->elements->size; i++) {
		fprintf(file, "        %-20s %p\n", rt->elements->tbl[i].name, rt->elements->tbl[i].ecb);
	}

	//Signal we have eneded the dump
	fprintf(file, "    DUMP FINISHED\n");
}
