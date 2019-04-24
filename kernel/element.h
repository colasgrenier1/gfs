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
	///The list of children of this element
	element_child_list * children;
	///Private use for the element.
	void * private;
	///Destructor of the private use area
	void (*destroy) (void *);
} element;

/**
 *
 */
element * element_new(const char * name, int line, int col);

/**
 * Free the element & its children elements.
 */
void element_free(element * elem);

/**
 * Add an element as a child to an element.
 */
void element_add_child(element * elem, element * child);

#endif
