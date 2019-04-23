#ifndef __PARSER
#define __PARSER

/*******************************************************************************
 *
 *                      T O K E N    D E F I N I T I O N
 *
 ******************************************************************************/

/**
 * Token type.
 */
typedef enum {
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
typedef struct {
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
typedef struct {
	token * tok;
	token_list_element * next;
} token_list_element;

///Token list
typedef struct {
	token_list_element * first;
	token_list_element * last;
} token_list;

///New list
token_list * token_list_new();

///Add a token to the list
void * token_list_add(token_list * list, token * tok);

///Free the token list
void token_list_free(token_list * list);

/*******************************************************************************
 *
 *                     P A R S I N G   F U N C T I O N S
 *
 ******************************************************************************/

/**
 * Parses the file content, returns the token list.
 */
error * parse(FILE * file, token_list ** list);

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