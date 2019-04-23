#ifndef __RUNTIME
#define __RUNTIME

#include "error.h"
#include "token.h"

/*******************************************************************************
 *                                                                             *
 *                         R U N T I M E   T A B L E S                         *
 *                                                                             *
 ******************************************************************************/

#define RUNTIME_EXTENSION_TABLE_DEFAULT_SIZE 200
#define RUNTIME_EXTENSION_TABLE_INCREMENT 40

typedef struct {
	int size;
	int alloc;
	char ** tbl;
} runtime_extension_table;

runtime_extension_table * runtime_extension_table_new();

error * runtime_extension_table_add(runtime_extension_table * tbl, char * name);

#define RUNTIME_ELEMENT_TABLE_DEFAULT_SIZE 200
#define RUNTIME_ELEMENT_TABLE_INCREMENT 40

typedef struct {
	char * name;
	error * (*free) (element * elem);
} runtime_element_table_elem;

typedef struct {
	int size;
	int alloc;
	runtime_element_table_elem * tbl;
} runtime_element_table;

runtime_element_table * runtime_element_table_new();

error * runtime_element_table_add(runtime_element_table * tbl,
	char * name, error * (*free) (element * elem));

error * runtime_element_table_setfree(runtime_element_table * tbl,
	char * name, error * (*free) (element * elem));

#define RUNTIME_COMMAND_TABLE_DEFAULT_SIZE 300
#define RUNTIME_COMMAND_TABLE_INCREMENT 100

typedef struct {
	char * name;
	error * (*execute)(document * doc, token_list * tokens);
} runtime_command_table_elem;

typedef struct {
	int size;
	int alloc;
	runtime_command_table_elem * tbl;
} runtime_command_table;

///Create a new runtime table.
runtime_command_table * runtime_command_table_new();

///Add to a runtime table.
error * runtime_command_table_add(runtime_command_table * tbl, char * name, error * (*execute)(document * doc, token_list * tokens));

#define RUNTIME_RENDERER_TABLE_DEFAULT_SIZE 300
#define RUNTIME_RENDERED_TABLE_INCREMENT 100

typedef struct {
	char * fmt;
	char * elem;
	error * (*render) (element * elem, FILE * file);
} runtime_renderer_table_elem;

typedef struct {
	int size;
	int alloc;
	runtime_renderer_table_elem * tbl;
} runtime_renderer_table;

///Create a renderer table.
runtime_renderer_table * runtime_renderer_table_new();

///Add an element to the renderer table.
error * runtime_renderer_table_add(runtime_renderer_table * tbl,
	char * elem, char * fmt, error * (*render)(element * elem, FILE * file));

///Returns the command pointer for the element & format.
error * (*render) (element * elem, FILE * file) runtime_rendered_table_get(runtime_renderer_table * tbl, char * fmt, char * elem);

/*******************************************************************************
 *                                                                             *
 *                               R U N T I M E                                 *
 *                                                                             *
 ******************************************************************************/

/**
 * The runtime possesses the elements loaded for this session.
 */
typdef struct {
	///Log output.
	FILE * log;
	///The plugin table
	runtime_extension_table * extensions;
	///The element table (contains name and destructors).
	runtime_element_table * elements;
	///The command table
	runtime_command_table * commands;
	///The renderer table
	runtime_renderer_table * renderers;
} runtime;

/**
 * Creates a runtime.
 */
runtime * runtime_new(FILE * log);

/*
 * Log a message.
 */
void runtime_log(runtime * rt, char * msg);

/**
 * Load an extension..
 */
error * runtime_load(runtime * rt, char * file);



/*******************************************************************************
 *
 *                          E X E C U T I O N
 *
 ******************************************************************************/

/**
 * Creates a document, filled with the runtime.
 */
document * runtime_document_new(runtime * rt);

/**
 * Execute a list of tokens on a document.
 */
error * document_execute(document * doc, token_list * tok);

/**
 * Parses a file and creates a document from it.
 */
error * document_from_file(runtime * rt, FILE *f);

/**
 * Renders a document.
 */
error * document_render(document * doc, char * format);

#endif
