#include <string.h>

#include "document.h"
#include "command.h"

/*******************************************************************************
 *                                                                             *
 *                      D O C U M E N T   E L E M E N T                        *
 *                                                                             *
 ******************************************************************************/

element_control_block * document_ecb_new() {

}



/*******************************************************************************
 *                                                                             *
 *D O C U M E N T   E L E M E N T   R U N T I M E   I N I T I A L I Z A T I O N*
 *                                                                             *
 ******************************************************************************/

error * document_initialize(runtime * rt) {

}

/*******************************************************************************
 *                                                                             *
 *                D O C U M E N T   E L E M E N T   S T A C K                  *
 *                                                                             *
 ******************************************************************************/

 document_stack * document_stack_new() {
	 document_stack * p = malloc(sizeof(document_stack));
	 p->top = NULL;
 }

 void document_stack_push(document_stack * stack, element * elem) {
	 document_stack_elem * n = malloc(sizeof(document_stack_elem));
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

 /*
  * Frame stack: contains name->command mappings.
  */

document_frame * document_frame_new() {
	document_frame * p = malloc(sizeof(document_frame));
	p->size = 0;
	p->alloc = DOCUMENT_FRAME_DEFAULT_SIZE;
	p->elems = malloc(sizeof(document_frame_elem)*DOCUMENT_FRAME_DEFAULT_SIZE);
	return p;
}

void document_frame_free(document_frame * f) {

}

void document_frame_add(document_frame * frame, char * name, command * cmd) {
	if (frame->size >= frame->alloc) {
		document_frame_elem * old = frame->elems;
		frame->elems = malloc(sizeof(document_frame_elem)*(frame->alloc+DOCUMENT_FRAME_INCREMENT));
		memcpy(frame->elems, old, sizeof(document_frame_elem)*(frame->size));
	}
	frame->elems[frame->size].name = name;
	frame->elems[frame->size].cmd = cmd;
	frame->size += 1;
}

command * document_frame_get(document_frame * frame, char * name) {
	int i;
	for (i=0; i<frame->size; i++) {
		if (strcmp(frame->elems[i].name, name) == 0) {
			return frame->elems[i].cmd;
		}
	}
	return NULL;
}

/*
 * Frame stack: contains the stack of frames and the toplevel.
 */

///Creates a frame stack, including the topvlevel.
document_frame_stack * document_frame_stack_new() {
	document_frame_stack * fs = (document_frame_stack*)malloc(sizeof(document_frame_stack));
	fs->toplevel = document_frame_new();
	fs->top = NULL;
	return fs;
}

void document_frame_stack_register(document_frame_stack * stack, char * name, command * cmd) {
	if (stack->top == NULL) {
		document_frame_add(stack->top->frame, name, cmd);
	} else {
		document_frame_add(stack->toplevel, name, cmd);
	}
}

void document_frame_stack_enter(document_frame_stack * stack) {
	document_frame * n = document_frame_new();
	document_frame_stack_elem * fe = (document_frame_stack_elem*)malloc(sizeof(document_frame_stack_elem));
	fe->frame = n;
	fe->bottom = NULL;
	if (stack->top == NULL) {
		stack->top = fe;
	} else {
		fe->bottom = stack->top;
		stack->top = fe;
	}
}

void document_frame_stack_leave(document_frame_stack * stack) {
	if (stack->top != NULL) {
		document_frame_stack_elem * old = stack->top;
		stack->top = stack->top->bottom;
		//We free the frame stack element
		document_frame_free(old->frame);
		//We free the actual frame stack
		free(old);
	}
}

command * document_frame_stack_resolve(document_frame_stack * stack, char * name) {
	//We try to find
	document_frame_stack_elem * cur = stack->top;
	while (cur != NULL) {
		command * c = document_frame_get(cur->frame, name);
		if (c != NULL) {
			return c;
		}
		cur = cur->bottom;
	}
	//We try the topleveel
	return document_frame_get(stack->toplevel, name);
}


/*******************************************************************************
 *                                                                             *
 *                              D O C U M E N T                                *
 *                                                                             *
 ******************************************************************************/

document * document_new() {
	document * d = (document*)malloc(sizeof(document));
	d->stack = document_stack_new();
	d->frames = document_frame_stack_new();
	return d;
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

void document_register(document * doc, command * cmd, char * name) {
	document_frame_stack_register(doc->frames, name, cmd);
}

command * document_resolve(document * doc, char * name) {
	return document_frame_stack_resolve(doc->frames, name);
}
