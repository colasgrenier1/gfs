#ifndef __RENDERER
#define __RENDERER

//forward
typedef struct renderer renderer;
typedef struct renderer_table renderer_table;

//import
#include "element.h"
#include "runtime.h"

/*******************************************************************************
 *                                                                             *
 *                    R E N D E R E R   S T R U C T U R E                      *
 *                                                                             *
 ******************************************************************************/

///The renderer structure
typedef struct renderer {
	///The name of the element this processes.
	char * element;
	///The format this processes.
	char * fmt;
	///Private area, to keep option data, etc.
	void * private;
	///Function to free the private area.
	void (*free)(renderer * r);
	///Function to set an option.
	error * (*set_option)(renderer * r, char * option, void * body);
	///The function to render an element.
	error * (*render)(runtime * rt, element * elem, FILE * file);
} renderer;

renderer * renderer_new();

///Set an option (kernel use)
error * renderer_set_option(renderer * r, char * option, void * body);

///Call the renderer (kernel use)
error * renderer_render(renderer * r, runtime * rt, element * elem, FILE * file);

///Free the renderer
void renderer_free(renderer * r);

/*******************************************************************************
 *                                                                             *
 *                        R E N D E R E R   T A B L E                          *
 *                                                                             *
 ******************************************************************************/

#define RENDERER_TABLE_DEFAULT_SIZE 1
#define RENDERER_TABLE_INCREMENT 1

typedef struct renderer_table {
	int size;
	int alloc;
	char ** formats;
	renderer ** renderers;
} renderer_table;

renderer_table * renderer_table_new();

void renderer_table_set(renderer_table * rt, char * fmt, renderer * r);

renderer * renderer_table_get(renderer_table * rt, char * fmt);

#endif
