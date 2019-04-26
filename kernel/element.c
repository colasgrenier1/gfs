#include <stdbool.h>
#include <stdlib.h>

#include "element.h"

/*******************************************************************************
 *                                                                             *
 *                      E L E M E N T   C H I L D R E N                        *
 *                                                                             *
 ******************************************************************************/

 element_child_list * element_child_list_new() {
	 element_child_list * p = malloc(sizeof(element_child_list));
	 p->first = NULL;
	 p->last = NULL;
	 return p;
 }

 void element_child_list_add(element_child_list * list, element * elem) {
	 element_child_list_elem * n = malloc(sizeof(element_child_list_elem));
	 n->elem = elem;
	 if (list->first == NULL) {
		 n->next = NULL;
		 list->first = n;
		 list->last = n;
	 } else {
		 list->last->next = n;
		 list->last = n;
	 }
 }

 void element_child_list_free(element_child_list * list) {
 }

/*******************************************************************************
 *                                                                             *
 *                               E L E M E N T                                 *
 *                                                                             *
 ******************************************************************************/

element * element_new(const char * name, element_control_block * ecb, int line, int col) {
	element * e = (element*)malloc(sizeof(element));
	e->name = name;
	e->ecb = ecb;
	e->line = line;
	e->col = col;
	e->can_pop = false;
	e->text_container = false;
	e->paragraph_container = false;
	e->children = element_child_list_new();
	e->private = NULL;
}

void element_free(element * elem) {
	if (elem->ecb->free_element != NULL) {
		elem->ecb->free_element(elem);
	}
	free(elem);
}

void element_add_child(element * elem, element * child) {
	element_child_list_add(elem->children, child);
}

/*******************************************************************************
 *                                                                             *
 *                 E L E M E N T   C O N T R O L   B L O C K                   *
 *                                                                             *
 ******************************************************************************/

element_control_block * element_control_block_new(char * name) {
	element_control_block * ecb = (element_control_block*)malloc(sizeof(element_control_block));
	ecb->name = name;
	ecb->renderers = renderer_table_new();
	ecb->private = NULL;
	ecb->set_option = NULL;
	ecb->free = NULL;
	ecb->free_element = NULL;
}
