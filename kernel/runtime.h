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

#include "command.h"
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
	element_control_block * ecb;
} runtime_element_table_elem;

typedef struct runtime_element_table {
	int size;
	int alloc;
	runtime_element_table_elem * tbl;
}  runtime_element_table;

runtime_element_table * runtime_element_table_new();

error * runtime_element_table_add(runtime_element_table * tbl, char * name, element_control_block * ecb);

element_control_block * runtime_element_table_get(runtime_element_table * tbl, char * name);

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
	/**
	 * Runtime tables.
	 */
	///A list of extension files
	runtime_extension_table * extensions;
	///The element table
	runtime_element_table * elements;
	///The table of commands
	command_table * commands;
	/**
	 * Builtin in element element control blocks.
	 */
	element_control_block * document_ecb;
	element_control_block * paragraph_ecb;
	element_control_block * text_ecb;
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
 * Load all the .gfx files in a directory
 */
error * runtime_loaddir(runtime * rt, char * dir);

/**
 * Frees a runtime and all its associated modules.
 */
void runtime_free(runtime * rt);

/*******************************************************************************
 *                                                                             *
 *                  R U N T I M E   R E G I S T R A T I O N                    *
 *                                                                             *
 ******************************************************************************/

/**
 * Register a command on the runtime.
 */
error * runtime_register_command(runtime * rt, char * name, command * cmd);

/**
 *
 */
error * runtime_register_element(runtime * rt, char * name, element_control_block * ecb);

/**
 * Get an element control block.
 */
element_control_block * runtime_element_control_block_get(runtime * rt, char *name);

/**
 * Register a renderer for a particular element and format.
 */
error * runtime_register_renderer(runtime * rt, char * elem, char * fmt, renderer * r);

/**
 * Send an option to an element of this runtime (specificfally the element control block).
 */
error * runtime_set_element_option(runtime * rt, char * elem, char * opt, void * body);

/**
 * Send an option to a renderer of this runtime.
 */
error * runtime_set_renderer_option(runtime * rt, char * elem, char * fmt, char *opt, void * body);

/*******************************************************************************
 *
 *                          E X E C U T I O N
 *
 ******************************************************************************/

/**
 * Creates a document, filled with the runtime.
 */
document * new_document(runtime * rt);

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
error * render(document * doc, char * format, FILE * file);

/**
 *
 */
error * render_element(element * elem, FILE * file);

/**
 *
 */
error * render_element_children(element * elem, char * fmt, FILE * file);

/*******************************************************************************
 *                                                                             *
 *                                 D E B U G                                   *
 *                                                                             *
 ******************************************************************************/

void * runtime_dump_tables(runtime * rt, FILE * file);

#endif
