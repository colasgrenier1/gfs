#ifndef __COMMAND
#define __COMMAND

#include "document.h"
#include "parser.h"

/*
 * A command structure.
 */
typedef struct{
	///A function
	error* (*execute)(command * cmd, document * doc, token_list * tok);
	///Private member
	void * private;
	///Function pointer to free this command's private area.
	void (*free)(void * private);
} command;

///Create a new command.
void command_new(error* (*execute)(command * cmd, document * doc, token_list * tok), void * private);

///Execute the command.
void command_execute(command * cmd, document * doc, token_list * tok);

#endif
