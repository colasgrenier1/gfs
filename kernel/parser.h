#ifndef __PARSER
#define __PARSER

#include <stdbool.h>

#include "element.h"
#include "error.h"

/*******************************************************************************
 *                                                                             *
 *                   F O W A R D   D E C L A R A T I O N S                     *
 *                                                                             *
 ******************************************************************************/

typedef enum token_type token_type;
typedef struct token token;
typedef struct token_list_element token_list_element;
typedef struct token_list token_list;

/*******************************************************************************
 *
 *                      T O K E N    D E F I N I T I O N
 *
 ******************************************************************************/

/**
 * Token type.
 */
typedef enum token_type {
	EMPTY = 0,
	NEWLINE = 1,
	SPACE = 2,
	WORD = 3,
	COMMAND = 4,
	BLOCK = 5
} token_type;

/**
 * A token.
 */
typedef struct token {
	///Type of the token
	token_type type;
	///Line number
	int line;
	///Column
	int col;
	///String content or token child list
	union {
			char * content;
			token_list * contents;
	};
} token;

/**
 * Allocate a token.
 */
token * token_alloc();

/**
 * Create new tokens.
 */
token * token_empty_new();
token * token_newline_new(int line, int col, char c);
token * token_space_new(int line, int col, char c);
token * token_word_new(int line, int col, char c);
token * token_command_new(int line, int col);
token * token_block_new();

/**
 * Append a character to the token's content member.
 * WARNING: does not check type! Do not use with BLOCK tokens.
 */
void token_content_append(char c);


/*******************************************************************************
 *
 *                         T O K E N    L I S T S
 *
 ******************************************************************************/

///Token list element.
typedef struct token_list_element {
	token * tok;
	token_list_element * next;
	token_list_element * previous;
} token_list_element;

///Token list
typedef struct token_list {
	token_list_element * first;
	token_list_element * last;
} token_list;

///New list
token_list * token_list_new();

///Returns if the list is empty.
bool token_list_empty(token_list * list);

///Add a token to the list
void * token_list_add(token_list * list, token * tok);

///Remove the first element from the list
token * token_list_pop(token_list * list);

///Remove the last element in the list.
token * token_list_remove_last(token_list * list);

///Returns the last element.
token * token_list_last(token_list * list);

///Duplicate a list, including its children elements.
token_list * token_list_dup(token_list * list);

///Free the token list
void token_list_free(token_list * list);

/*******************************************************************************
 *
 *                     P A R S I N G   F U N C T I O N S
 *
 ******************************************************************************/

/**
 * Parses the file content, returns the token list in the buffer.
 */
error * parse(FILE * file, int * line, int * col, token_list ** list, bool inblock);

/*******************************************************************************
 *
 *                      D E B U G    F U N C T I O N S
 *
 ******************************************************************************/

///Get the string representation of a token name
const char * token_type_name(token_type type);

///Display a token list to a file
void token_list_dump(FILE * file, token_list * list);

#endif
