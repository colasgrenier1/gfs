#include <stdbool.h>

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

 void element_child_list_free(element_child_list) {
 }

 /*******************************************************************************
  *                                                                             *
  *                               E L E M E N T                                 *
  *                                                                             *
  ******************************************************************************/
