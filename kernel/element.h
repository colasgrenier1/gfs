#ifndef ELEMENT
#define ELEMENT

#include <stdbool.h>

/*******************************************************************************
 *                                                                             *
 *                  I N I T I A L   D E C L A R A T I O N S                    *
 *                                                                             *
 ******************************************************************************/

typedef struct element element;
typedef struct element_child_list element_child_list;
typedef struct element_child_list_elem element_child_list_elem;
typedef struct element_control_block element_control_block;

/*******************************************************************************
 *                                                                             *
 *                          D E P E N D E N C I E S                            *
 *                                                                             *
 ******************************************************************************/

#include "error.h"
#include "renderer.h"

/*******************************************************************************
 *                                                                             *
 *                      E L E M E N T   C H I L D R E N                        *
 *                                                                             *
 ******************************************************************************/

/**
 * An element list element.
 */
typedef struct element_child_list_elem {
	element * elem;
	element_child_list_elem * next;
} element_child_list_elem;

/**
 * A list of elements.
 */
typedef struct element_child_list {
	element_child_list_elem * first;
	element_child_list_elem * last;
} element_child_list;

/**
 * Creates an element child list.
 */
element_child_list * element_child_list_new();

/**
 *
 */
bool element_child_list_empty(element_child_list * list);

/**
 * Adds an element to a child list.
 */
void element_child_list_add(element_child_list * list, element * elem);

/**
 * Frees the element child list.
 * NOTE: does not calls element_free() on each child element!.
 */
void element_child_list_free(element_child_list * list);

/*******************************************************************************
 *                                                                             *
 *                               E L E M E N T                                 *
 *                                                                             *
 ******************************************************************************/

/**
 * A document element.
 */
typedef struct element {
	///Unique name of the element type.
	const char * name;
	///The element control block associated with this element (for speed)
	element_control_block * ecb;
	///Line at which this element begins
	int line;
	///Column at which this element begins.
	int col;
	///Wether this element can be removed in search of another.
	bool can_pop;
	///Wether this element can hold text
	bool text_container;
	///Whether this element can hold paragraphs.
	bool paragraph_container;
	///Wether a new paragraph should be considered
	///NOTE: This is not a variable reserved for the executor's
	///      use only. It is used to keep track of whether an empty
	///      line has just been read while the stack pointed to
	///      this element.
	bool maybe_paragraph_required;
	///The list of children of this element
	element_child_list * children;
	///Private use for the element.
	void * private;
} element;

/**
 *
 */
element * element_new(const char * name, element_control_block * ecb, int line, int col);

/**
 * Free the element & its children elements.
 */
void element_free(element * elem);

/**
 * Add an element as a child to an element.
 */
void element_add_child(element * elem, element * child);

/*******************************************************************************
 *                                                                             *
 *                 E L E M E N T   C O N T R O L   B L O C K                   *
 *                                                                             *
 ******************************************************************************/

typedef struct element_control_block {
	///The name of the element
	char * name;
	///The renderer table for this element control block.
	renderer_table * renderers;
	///Pointer to the private area of this control block
	void * private;
	///Set an option
	error * (*set_option)(element_control_block * ecb, char * option, void * content);
	///Free this ECB
	void (*free)(element_control_block * ecb);
	///A function to free an instance of this element
	void (*free_element)(element * elem);
} element_control_block;

element_control_block * element_control_block_new(char * name);

#endif
