#include <stdbool.h>

#include "element.h"
#include "runtime.h"

/*******************************************************************************
 *                                                                             *
 *                    F O W A R D   D E C L A R A T I O N                      *
 *                                                                             *
 ******************************************************************************/

typedef struct text_private text_private;
typedef struct text_private_elem text_private_elem;

/*******************************************************************************
 *                                                                             *
 *                                R E N D E R                                  *
 *                                                                             *
 ******************************************************************************/

error * text_render_html(element * elem, FILE * file) {

}

error * text_render_latex(element * elem, FILE * file) {

}

error * text_render_xml(element * elem, FILE * file) {

}

/*******************************************************************************
 *                                                                             *
 *                               C O N T R O L                                 *
 *                                                                             *
 ******************************************************************************/

element_control_block * text_ecb_new() {

}

element * text_new() {

}

error * text_initialize(runtime * rt) {
	runtime_register_element(rt, "text", text_ecb_new());
}

/*******************************************************************************
 *                                                                             *
 *                          T E X T   M E M B E R S                            *
 *                                                                             *
 ******************************************************************************/

//Text buffer parameters.
#define TEXT_PRIVATE_ELEM_DEFAULT_SIZE 10
#define TEXT_PRIVATE_ELEM_INCREMENT 10

/**
 * Type of text element.
 */
typedef enum text_type {
	TEXT_WORD,
	TEXT_SPACE,
	TEXT_NBSPACE,
	TEXT_PUNCTUATION
} text_type;

/**
 * Linked list member.
 */
typedef struct text_private_elem {
	text_type type;
	char * value;
	text_private_elem * previous;
	text_private_elem * next;
} text_private_elem;

void text_private_elem_append_char(text_private_elem * tpe, char c) {
	if (tpe->value == NULL) {
		tpe->value = malloc(sizeof(char)*2);
		tpe->value[0] = c;
		tpe->value[1] = 0;
	} else {
		char * old = tpe->value;
		tpe->value = malloc(sizeof(char)*(strlen(tpe->value)+2));
		strcpy(tpe->value, old);
		strncat(tpe->value, &c, 1);
		free(old);
	}
}

void text_private_elem_append_str(text_private_elem * tpe, char * str) {
	//TODO horrendously inefficient: redo!
	int i;
	for(i=0; i<strlen(str); i++) {
		text_private_elem_append_char(str[i]);
	}
}

/**
 * Private area for the text element.
 */
typedef struct text_private {
	text_private_elem * first;
	text_private_elem * last;
} text_private;

text_private * text_private_new() {
	text_private * tp = (text_private*)malloc(sizeof(text_private));
	tp->first = NULL;
	tp->last = NULL;
}

void text_private_free(text_private * tp) {
	text_private_elem * cur = tp->first;
	text_private_elem * old;
	while (cur != NULL) {
		free(cur->value);
		old = cur;
		cur = cur->next;
		free(old);
	}
}

void text_private_add(text_private * tp, text_type type) {
	text_private_elem * tpe = (text_private_elem*)malloc(sizeof(text_private_elem));
	tpe->type = type;
	tpe->next = NULL;
	tpe->previous = NULL;
	if (tp->last != NULL) {
		tpe->previous = tp->last;
		tp->last = tpe;
	} else {
		tp->first = tpe;
		tp->last = tpe;
	}
}

text_private_elem * text_private_last(text_private * tp) {
	return tp->last;
}

text_private_elem * text_private_nlast(text_private * tp, int n) {

}

void text_private_remove_last(text_private * tp) {
	if (tp->last != NULL) {
		free(tp->last->value);
		if (tp->last->previous == NULL) {
			free(tp->last);
			tp->first = NULL;
			tp->last = NULL;
		} else {
			tp->last = tp->last->previous;
			free(tp->last);
		}
	}
}

/*******************************************************************************
 *                                                                             *
 *                                  T E X T                                    *
 *                                                                             *
 ******************************************************************************/

/**
 * Add a character to a text element. Will process puncutation, etc.
 */
error * text_add_character(element * elem, char c) {
	text_private * tp = (text_private*)(elem->private);
	//Right now, completely dumb.
	if (!text_private_empty(tp) && text_private_last(tp)->type==TEXT_WORD) {
		text_private_elem_append_char(text_private_last(tp), c);
	} else {
		text_private_add(tp, TEXT_WORD);
		text_private_elem_append_char(text_private_last(tp), c);
	}
}

/**
 * Adds a space to a text element.
 */
error * text_add_space(element * elem) {
	text_private * tp = (text_private*)(elem->private);
	if (!text_private_empty(tp) && text_private_last(tp)->type==TEXT_SPACE) {
		//Do nothing
	} else {
		text_private_add(tp, TEXT_SPACE);
	}
}

/*******************************************************************************
 *                                                                             *
 *                R U N T I M E   I N I T I A L I Z A T I O N                  *
 *                                                                             *
 ******************************************************************************/

error * text_initialize(runtime * rt) {
	
}
