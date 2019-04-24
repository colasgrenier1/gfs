#include <dirent.h>
#include <dlfcn.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "runtime.h"
#include "error.h"

/*******************************************************************************
 *                                                                             *
 *                         R U N T I M E   T A B L E S                         *
 *                                                                             *
 ******************************************************************************/

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
	 tbl->size += 1;
	 tbl->tbl[tbl->size] = name;
	 return NULL;
 }

runtime_element_table * runtime_element_table_new() {
	runtime_element_table * p = malloc(sizeof(runtime_element_table));
	p->size = 0;
	p->tbl = calloc(sizeof(runtime_element_table_elem), RUNTIME_ELEMENT_TABLE_DEFAULT_SIZE);
	p->alloc = RUNTIME_ELEMENT_TABLE_DEFAULT_SIZE;
	return p;
}

error * runtime_element_table_add(runtime_element_table * tbl, char * name, error * (*free) (element * elem)) {
	//If we do not have space we add space
	if (tbl->size >= tbl->alloc) {
		//We readjust
		runtime_element_table_elem * tmp = tbl->tbl;
		int tmps = tbl->size;
		tbl->tbl = calloc(sizeof(runtime_element_table_elem), tbl->alloc + RUNTIME_ELEMENT_TABLE_INCREMENT);
		memcpy(tbl->tbl, tmp, tmps*sizeof(runtime_element_table_elem));
	}
	tbl->tbl[tbl->size].name = name;
	tbl->tbl[tbl->size].free = free;
	tbl->size += 1;
	return NULL;
}

error * runtime_element_table_setfree(runtime_element_table * tbl, char * name, error * (*free) (element * elem)) {
	int i;
	for (i=0; i<tbl->size; i++) {
		if (strcmp(tbl->tbl[i].name, name)==0) {
			tbl->tbl[i].free = free;
			return NULL;
		}
	}
	return NULL;
}

runtime_command_table * runtime_command_table_new() {
	runtime_command_table * tbl = malloc(sizeof(runtime_command_table));
	tbl->size = 0;
	tbl->alloc = RUNTIME_COMMAND_TABLE_DEFAULT_SIZE;
	tbl->tbl = malloc(sizeof(runtime_command_table_elem)*RUNTIME_COMMAND_TABLE_DEFAULT_SIZE);
	return tbl;
}

error * runtime_command_table_add(runtime_command_table * tbl, char * name, error * (*execute)(document * doc, token_list * tokens)) {
	//we add space if there is none
	if (tbl->size >= tbl->alloc) {
		runtime_command_table_elem * old = tbl->tbl;
		tbl->tbl = malloc(sizeof(runtime_element_table_elem)*(tbl->alloc+RUNTIME_COMMAND_TABLE_INCREMENT));
		memcpy(tbl->tbl,old,tbl->size*sizeof(runtime_element_table_elem));
		free(old);
	}
	//We add te element
	tbl->tbl[tbl->size].name = name;
	tbl->tbl[tbl->size].execute = execute;
	tbl->size += 1;
	return NULL;
}

runtime_renderer_table * runtime_renderer_table_new() {
	runtime_renderer_table * p = malloc(sizeof(runtime_renderer_table));
	p->size = 0;
	p->alloc = RUNTIME_RENDERER_TABLE_DEFAULT_SIZE;
	p->tbl = malloc(sizeof(runtime_renderer_table_elem)*RUNTIME_RENDERER_TABLE_DEFAULT_SIZE);
	return p;
}

error * runtime_renderer_table_add(runtime_renderer_table * tbl, char * elem, char * fmt, error * (*render)(element * elem, FILE * file)) {
	//We add space if there is not enough
	if (tbl->size >= tbl->alloc) {
		runtime_renderer_table_elem * old = tbl->tbl;
		tbl->tbl = malloc(sizeof(runtime_renderer_table_elem)*(tbl->alloc+RUNTIME_RENDERED_TABLE_INCREMENT));
		memcpy(tbl->tbl,old,tbl->size*sizeof(runtime_renderer_table_elem));
		free(old);
	}
	tbl->tbl[tbl->size].fmt = fmt;
	tbl->tbl[tbl->size].elem = elem;
	tbl->tbl[tbl->size].render = render;
	tbl->size += 1;
}

//error * (*render) (element * elem, FILE * file) runtime_rendered_table_get(runtime_renderer_table * tbl, char * fmt, char * elem) {
error* (*runtime_renderer_table_get(runtime * rt, char * fmt, char * elem))(element * elem, FILE * file) {

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
	 rt->commands = runtime_command_table_new();
	 rt->renderers = runtime_renderer_table_new();
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
		return error_new(1002, "COULD NOT LOAD PLUGIN \'%s\': %s", file, dlerror());
	}

	//We add the library to the runtime
	runtime_extension_table_add(rt->extensions, file);

	//We then initialize the library
	//TODO

	//We first get the element list.
	//It is simply an array of strings.
	tmp = dlsym(ptr, "ELEMENTS");
	if (tmp == NULL) {
		return error_new(1010, "NO ELEMENT TABLE IN PLUGIN '%s'", file);
	}
	char ** elist = (char **)tmp;
	//We fill the table
	idx = 0;
	while (elist[idx] != NULL) {
		runtime_element_table_add(rt->elements, elist[idx], NULL);
		idx += 1;
	}

	//We then get the destructor list
	//A list of string then pointer.
	tmp = dlsym(ptr, "DESTRUCTORS");
	if (tmp==NULL) {
		return error_new(1011, "NO DESTRUCTOR TABLE IN PLUGIN '%s'", file);
	}
	void ** dlist = (void **)tmp;
	//We fill the table
	idx = 0;
	while (dlist[idx] != NULL) {
		if ((idx % 2) == 0) {
			tmp1 = (char *)dlist[idx];
		} else {
			runtime_element_table_setfree(rt->elements, tmp1, (error* (*)(element * elem))dlist[idx]);
		}
		idx += 1;
	}

	//We then get the command list.
	//It is organised as a long list, with first a string
	//then a pointer to the function.
	tmp = dlsym(ptr, "COMMANDS");
	if (tmp==NULL) {
		return error_new(1012, "NO COMMAND TABLE IN PLUGIN '%s'", file);
	}
	void ** cmdlist = (void **)tmp;
	//We fill the table
	idx = 0;
	while (cmdlist[idx] != NULL) {
		if ((idx%2) == 0) {
			tmp1 = (char *)cmdlist[idx];
		} else {
			runtime_command_table_add(rt->commands, tmp1, (error* (*)(document * doc, token_list * tok))cmdlist[idx]);
		}
		idx += 1;
	}

	//We then get the renderer list.
	//The first is a string for element name
	//then a string for the format type
	//finally a pointer to the function
	tmp = dlsym(ptr, "RENDERERS");
	if (tmp==NULL) {
		return error_new(1013, "NO RENDERER TABLE IN PLUGIN '%s'", file);
	}
	void ** rlist = (void **)tmp;
	//We fill the table+
	idx = 0;
	while (rlist[idx] != NULL) {
		if ((idx%3) == 0) {
			tmp1 = (char *)rlist[idx];
		} else if ((idx%3) == 1) {
			tmp2 = (char *)rlist[idx];
		} else {
			runtime_renderer_table_add(rt->renderers, tmp1, tmp2, (error* (*)(element * elem, FILE * file))rlist[idx]);
		}
		idx += 1;
	}
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
		return error_new(12, "COULD NOT OPEN DIRECTORY '%s'", dir);
	}
}

void runtime_dump_tables(runtime * rt, FILE * f) {
	int idx;

	fprintf(f, "DUMPING RUNTIME TABLES FOR RUNTIME %p\n", (void*)rt);

	fprintf(f, "  DUMPING %d ELEMENT(S)\n", rt->elements->size);
	for (idx=0; idx<rt->elements->size; idx++) {
		fprintf(f, "    %-20s %p\n", rt->elements->tbl[idx].name, (void*)rt->elements->tbl[idx].free);
	}

	fprintf(f, "  DUMPING %d COMMAND(S)\n", rt->commands->size);
	for (idx=0; idx<rt->commands->size; idx++) {
		fprintf(f, "    %-20s %p\n", rt->commands->tbl[idx].name, rt->commands->tbl[idx].execute);
	}

	fprintf(f, "  DUMPING %d RENDERER(S)\n", rt->renderers->size);
	for (idx=0; idx<rt->renderers->size; idx++) {
		fprintf(f, "    %-10s %-20s %p\n", rt->renderers->tbl[idx].fmt, rt->renderers->tbl[idx].elem, rt->renderers->tbl[idx].render);
	}
}

/*******************************************************************************
 *
 *                          E X E C U T I O N
 *
 ******************************************************************************/

error * render_element_children(runtime * rt, element * elem, char * fmt, FILE * file) {
	return NULL;
}
