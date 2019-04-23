#ifndef __ELEMENT
#define __ELEMENT

#include <stdbool.h>

/**
 * An element list element.
 */
typedef struct {
	element * elem;
	element_child_list_elem * next;
} element_child_list_elem;

/**
 * A list of elements.
 */
typedef struct {
	element_child_list_elem * first;
	element_child_list_elem * last;
} element_child_list;

/**
 * Adds an element to a child list.
 */
void element_child_list_add(element_child_list * list, element * elem);

/**
 * A document element.
 */
typedef struct {
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
} element;

/**
 *
 */
element * element_new(const char * name);

/**
 * Free the element.
 */
void element_free(element * elem);

/**
 * Add an element as a child to an element.
 */
void element_add_child(element * elem, element * elem);

#endif
