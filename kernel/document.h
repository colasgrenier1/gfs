#ifndef __DOCUMENT
#define __DOCUMENT

/*******************************************************************************
 *                                                                             *
 *                   F O W A R D   D E C L A R A T I O N S                     *
 *                                                                             *
 ******************************************************************************/

typedef struct document_stack_elem document_stack_elem;
typedef struct document_stack document_stack;
typedef struct document_frame_elem document_frame_elem;
typedef struct document_frame document_frame;
typedef struct document_frame_stack_elem document_frame_stack_elem;
typedef struct document document;

/*******************************************************************************
 *                                                                             *
 *                          D E P E N D E N C I E S                            *
 *                                                                             *
 ******************************************************************************/

 #include "command.h"
 #include "element.h"

/*******************************************************************************
 *                                                                             *
 *                D O C U M E N T   E L E M E N T   S T A C K                  *
 *                                                                             *
 ******************************************************************************/

typedef struct document_stack_elem {
	element * elem;
	document_stack_elem * prev;
} document_stack_elem;

typedef struct document_stack {
	document_stack_elem * top;
} document_stack;

document_stack * document_stack_new();

///Push onto the document stack.
void document_stack_push(document_stack * stack, element * elem);

///Get the current document element.
element * document_stack_top(document_stack * stack);

///Pop from the document stack.
element * document_stack_pop(document_stack * stack);


/*******************************************************************************
 *                                                                             *
 *       D O C U M E N T   C O N T E X T / V A R I A B L E   S T A C K         *
 *                                                                             *
 ******************************************************************************/

#define DOCUMENT_FRAME_DEFAULT_SIZE 50
#define DOCUMENT_FRAME_INCREMENT 10

typedef struct document_frame_elem {
	char * name;
	command * cmd;
} document_frame_elem;

///Contains variables
typedef struct document_frame {
	int size;
	int alloc;
	document_frame_elem * elems;
} document_frame;

///Creates a new frame
document_frame * document_frame_new();

///Add a command to a frame
void document_frame_add(document_frame * frame, char * name, command * cmd);

typedef struct document_frame_stack_elem {
	document_frame * frame;
	document_frame_stack_elem * bottom;
} document_frame_stack_elem;

typedef struct document_frame_stack {
	document_frame * topevel;
	document_frame_stack_elem * top;
} document_frame_stack;

///Create a frame stack.
document_frame_stack * document_frame_stack_new();

///Enter a block
void document_frame_stack_enter(document_frame_stack * stack);

///Leave a block
void document_frame_stack_leave(document_frame_stack * stack);

///Add a command
void document_frame_stack_add(document_frame_stack * stack, char * name, command * cmd);

///Resolve a command
command * document_frame_stack_resolve(document_frame_stack * stack, char * name);


/*******************************************************************************
 *
 *                            D O C U M E N T
 *
 ******************************************************************************/

/**
 * One master document object.
 */
typedef struct document {
	///Stack of elements.
	document_stack * stack;
	///Stack of frames (variables).
	document_frame_stack * frames;
} document;

///Allocates a document but does not do intiialization: use runtime_document_mew.
document * document_new();

///Returns the current top of the document stack.
element * document_top(document * top);

///Adds an element to the document stack.
void document_push(document * doc, element * elem);

///Pops an element from the document stack.
element * document_pop(document * doc);

 ///Adds a new frame to the document.
void document_enter(document * doc);

///Registers a new command.
void document_register(document * doc, command * cmd, char * name);

///Resolves a command or NULL.
command *  document_resolve(document * doc, char * name);

#endif
