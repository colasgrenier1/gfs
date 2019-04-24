#ifndef __RUNTIME
#define __RUNTIME

#include <stdarg.h>

/*******************************************************************************
 *                                                                             *
 *                   F O W A R D   D E C L A R A T I O N S                     *
 *                                                                             *
 ******************************************************************************/

typedef struct runtime_extension_table runtime_extension_table;
typedef struct runtime_element_table_elem runtime_element_table_elem;
typedef struct runtime_element_table runtime_element_table;
typedef struct runtime_command_table_elem runtime_command_table_elem;
typedef struct runtime_command_table runtime_command_table;
typedef struct runtime_renderer_table_elem runtime_renderer_table_elem;
typedef struct runtime_renderer_table runtime_renderer_table;
typedef struct runtime runtime;

/*******************************************************************************
 *                                                                             *
 *                          D E P E N D E N C I E S                            *
 *                                                                             *
 ******************************************************************************/

 #include "document.h"
 #include "element.h"
 #include "error.h"
 #include "parser.h"


/*******************************************************************************
 *                                                                             *
 *                         R U N T I M E   T A B L E S                         *
 *                                                                             *
 ******************************************************************************/

#define RUNTIME_EXTENSION_TABLE_DEFAULT_SIZE 200
#define RUNTIME_EXTENSION_TABLE_INCREMENT 40

typedef struct runtime_extension_table {
	int size;
	int alloc;
	char ** tbl;
} runtime_extension_table;

runtime_extension_table * runtime_extension_table_new();

error * runtime_extension_table_add(runtime_extension_table * tbl, char * name);

#define RUNTIME_ELEMENT_TABLE_DEFAULT_SIZE 200
#define RUNTIME_ELEMENT_TABLE_INCREMENT 40

typedef struct runtime_element_table_elem {
	char * name;
	error * (*free) (element * elem);
} runtime_element_table_elem;

typedef struct runtime_element_table {
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

typedef struct runtime_command_table_elem {
	char * name;
	error * (*execute)(document * doc, token_list * tokens);
} runtime_command_table_elem;

typedef struct runtime_command_table {
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

typedef struct runtime_renderer_table_elem {
	char * fmt;
	char * elem;
	error * (*render) (element * elem, FILE * file);
} runtime_renderer_table_elem;

typedef struct runtime_renderer_table {
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
error* (*runtime_renderer_table_get(runtime * rt, char * fmt, char * elem))(element * elem, FILE * file);

/*******************************************************************************
 *                                                                             *
 *                               R U N T I M E                                 *
 *                                                                             *
 ******************************************************************************/

/**
 * The runtime possesses the elements loaded for this session.
 */
typedef struct runtime {
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
void runtime_log(runtime * rt, char * fmt, ...);

/**
 * Load an extension..
 */
error * runtime_load(runtime * rt, char * file);

/**
 * Frees a runtime and all its associated modules.
 */
void runtime_free(runtime * rt);


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
 * The main processing function.
 */
error * execute(document * doc, token_list * tok);

/**
 * Parses a file and creates a document from it.
 */
error * document_from_file(runtime * rt, FILE *f);

/**
 * Renders a document.
 */
error * document_render(document * doc, char * format);

#endif
