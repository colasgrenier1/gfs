#include "document.h"

/*******************************************************************************
 *                                                                             *
 *                D O C U M E N T   E L E M E N T   S T A C K                  *
 *                                                                             *
 ******************************************************************************/

 element_stack * document_stack_new() {
	 element_stack * p = malloc(sizeof(element_stack));
	 p->top = NULL;
 }

 void document_stack_push(document_stack * stack, element * elem) {
	 document_stack_elem * n = malloc(sizeof(document_stack_elem))
	 n->elem = elem;
	 n->prev = NULL;
	 if (stack->top == NULL) {
		 stack->top = n;
	 } else {
		 n->prev = stack->top;
		 stack->top = n;
	 }
 }

 element * document_stack_top(document_stack * stack) {
	 if (stack->top==NULL) {
		 return NULL;
	 } else {
		 return stack->top->elem;
	 }
 }

 element * document_stack_pop(document_stack * stack) {
	 if (stack->top==NULL) {
		 return NULL;
	 } else {
		 element * tmp = stack->top->elem;
		 document_stack_elem * tmp2 = stack->top;
		 stack->top = stack->top->prev;
		 free(tmp2);
		 return tmp;
	 }
 }


/*******************************************************************************
 *                                                                             *
 *       D O C U M E N T   C O N T E X T / V A R I A B L E   S T A C K         *
 *                                                                             *
 ******************************************************************************/


/*******************************************************************************
 *                                                                             *
 *                              D O C U M E N T                                *
 *                                                                             *
 ******************************************************************************/

 document * document_new() {

 }

void document_push(document * doc, element * elem) {
	document_stack_push(doc->stack, elem);
}

element * document_top(document * doc) {
	return document_stack_top(doc->stack);
}

element * document_pop(document * doc) {
	return document_stack_pop(doc->stack);
}

void document_enter(document * doc) {
	document_frame_stack_enter(doc->frames);
}

void document_leave(document * doc) {
	document_frame_stack_leave(doc->frames);
}

void document_register(document * doc, (error * (*execute)(document * doc, token_list * tokens))) {

}

error * (*)(document * doc, token_list * tokens) document_resolve(document * doc, char * name) {

}
