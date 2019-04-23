#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "parser.h"

/*******************************************************************************
 *                                                                             *
 *                      T O K E N   D E F I N I T I O N                        *
 *                                                                             *
 ******************************************************************************/

token * token_alloc() {
	return (token*) malloc(sizeof(token));
}

void token_content_append(token * tok, char c) {
	char * nb = (char*)malloc(sizeof(char)*(strlen(tok->content)+4));
	strcpy(nb, tok->content);
	strcat(nb, &c, 1);
	free(tok->content);
	tok->content = nb;
}

/*******************************************************************************
 *                                                                             *
 *                            T O K E N   L I S T                              *
 *                                                                             *
 ******************************************************************************/

token_list * token_list_new() {
	token_list * p = (token_list*)malloc(sizeof(token_list));
	p->first = NULL;
	p->last = NULL;
	return p;
}

bool token_list_empty(token_list * list) {
	if ((list->first == NULL) || (list->last == NULL)) {
		return true;
	} else {
		return false;
	}
}

void token_list_add(token_list * list, token * tok) {
	token_list_element * n = (token_list_element*)malloc(sizeof(token_list_element));
	n->tok = tok;
	n->next = NULL;
	n->previous = NULL;
	if (list->first == NULL) {
		list->first = n;
		list->last = n;
	} else {
		list->last->next = n;
		n->previous = list->last;
		list->last = n;
	}
}

token * token_list_pop(token_list * list) {
	if (list->first == NULL) {
		return NULL;
	} else {
		if (list->first->next == NULL) {
			token * tmp = list->first->tok;
			token_list_element * old = list->first;
			list->first = NULL;
			list->last = NULL;
			free(old);
			return tmp;
		} else {
			token * tmp = list->first->tok;
			token_list_element * old = list->first;
			list->first = list->first->next;
			free(old);
			return tmp;
		}
	}
}

token * token_list_last(token_list * list) {
	if (list->last==NULL) {
		return NULL;
	} else {
		token * tmp = list->last->tok;
		token_list_element * old = list->last;
		if (list->last->previous == NULL) {
			list->first = NULL;
			list->last = NULL;
		} else {
			list->last = list->last->previous;
		}
		free(old);
		return tmp;
	}
}

token * token_list_remove_last(token_list * list) {
	if (list->last == NULL) {
		return NULL;
	} else {
		token * tmp = list->last->tok;

	}
}

token_list * token_list_dup(token_list * list) {
	token_list * newlist = token_list_new();
	token_list_elem * cur = list->first;
	while (cur != NULL) {
		token * t = token_alloc();
		t->type = cur->tok->type;
		if (t->type == BLOCK) {
			t->contents = token_list_dup(cur->contents);
		} else {
			t->content = t->content;
		}
		token_list_add(newlist, t);
		cur = cur->next;
	}
	return newlist;
}

void token_list_free(token_list * list) {

}

/*******************************************************************************
 *                                                                             *
 *                      P A R S I N G   F U N C T I O N                        *
 *                                                                             *
 ******************************************************************************/

error * parse(FILE * file, int * line, int * col, token_list ** list, bool inblock) {
	token_list * l = token_list_new();
	while (1) {
		//Read a character, if not end of file.
		int r = fgetc(file);
		if (r==EOF) {
			//Free the list since there is an error
			token_list_free(l)
			if (inblock) {
				//There is a syntax error.
				return error_new(301, "END OF FILE REACHED WHEN INSIDE A BLOCK : EXPECTING '>'");
			} else {
				*list = l;
				return NULL;
			}
		}
		//Now switch on the character
		char c = (char)r;
		if (c=='\n') {
			if (!token_list_empty(l) && token_list_last(l)->type==NEWLINE && strcmp(token_list_last(l)->content, "\r")) {
				token_content_append(token_list_last(l), c);
			} else {
				token_list_add(l, token_newline_new(*line, *col, c));
			}
			*line += 1;
			*col = 1;
		} else if (c=='\r') {
			//Not really a newline
			token_list_add(l, token_newline_new(*line, *col, c));
			*col += 1; //wait for \n to increment line
		} else if (c==' ' || c=='\t') {
			if (!token_list_empty(l) && token_list_last(l)->type==SPACE) {
				token_content_append(token_list_last(l), c);
			} else {
				token_list_add(l, token_spae_new(*line, *col, c));
			}
			//Tabs are 8 character
			if (c=='\t') {
				*col = (col + 8) % (col/8);
			} else {
				*col += 1;
			}
		} else if (c=='@') {
			if (!token_list_empty(l) && token_list_last(l)->type==COMMAND && strlen(token_list_last(l)->content)==0) {
				//If the previous was a command with no content then this is a literal
				//We remove the empty command.
				free(token_list_remove_last(l));
				//We check if the last was a word
				if (!token_list_empty(l) && token_list_last(l)->type==WORD) {
					token_content_append(token_list_last(l), '@');
				} else {
					token_list_add(l, token_word_new(*line, *col, '@'));
				}
			} else {
				//We begin a command
				token_list_add(l, token_command_new(*line, *col));
			}
		} else if (c=='<') {
			if(!token_list_empty(l) && token_list_last(l)->type==COMMAND && strlen(token_list_last(l)->content)==0) {
				//If the one before was '@' we
				free(token_list_remove_last(l));
				if (!token_list_empty(l) && token_list_last(l)->type==WORD) {
					token_content_append(token_list_last(l), '<');
				} else {
					token_list_add(l, token_word_new(*line, *col, '<'));
				}
			} else {
				//We begin a block
				token_list * child = token_list_new();
				//We try to parse
				error * e = parse(file, line, col, &child);
				if (e != NULL) {
					//The parsing has failed
					//We must cleanup and return an error
					token_list_free(child);
					token_list_free(l);
					return e;
				} else {
					//The parsing has been successful
					token * n = token_block_new(*line, *col);
					n->contents = child;
					token_list_add(l, n);
				}
			}
		} else if (c=='>') {
			if(!token_list_empty(l) && token_list_last(l)->type==COMMAND && strlen(token_list_last(l)->content)==0) {
				//If the one before was '@' we
				free(token_list_remove_last(l));
				if (!token_list_empty(l) && token_list_last(l)->type==WORD) {
					token_content_append(token_list_last(l), '<');
				} else {
					token_list_add(l, token_word_new(*line, *col, '<'));
				}
			} else {
				//This signifies we have ended a block
				if (inblock) {
					//Everything is OK.
					*list = l;
					return NULL
				} else {
					//Fail, we cleanup and return the error.
					token_list_free(l);
					return error_new(302, "'>' REACHED WHEN NOT INSIDE A BLOCK")
				}
			}
		} else if (c==';') {
			if (!token_list_empty(l) && token_list_last(l)->type=COMMAND) {
				token_list_add(l, token_empty_new());
			} else {
				if (!token_list_empty(l) && token_list_last(l)->type==WORD) {
					token_content_append(token_list_last(l), c);
				} else {
					token_list_add(l, token_word_new(*line, *col, c));
				}
			}
		} else {
			if (!token_list_empty(l) && token_list_last(l)->type==WORD) {
				token_content_append(token_list_last(l), c);
			} else {
				token_list_add(l, token_word_new(*line, *col, c));
			}
		}
	}
}
