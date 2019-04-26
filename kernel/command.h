#ifndef __COMMAND
#define __COMMAND

//Foward declaration
typedef struct command command;
typedef struct command_table command_table;

//includes
#include "document.h"
#include "parser.h"

/*******************************************************************************
 *                                                                             *
 *                               C O M M A N D                                 *
 *                                                                             *
 ******************************************************************************/

typedef struct command {
	///A function
	error* (*execute)(document * doc, token_list * tok, void * private);
	///Private member
	void * private;
	///Function pointer to free this command's private area.
	void (*free)(void * private);
} command;

///Create a new command.
command * command_new(error* (*execute)(document * doc, token_list * tok, void * private), void * private, void (*free)(void * private));

///Execute the command.
error * command_execute(command * cmd, document * doc, token_list * tok);

/*******************************************************************************
 *                                                                             *
 *                         C O M M A N D   T A B L E                           *
 *                                                                             *
 ******************************************************************************/

 #define COMMAND_TABLE_DEFAULT_SIZE 20
 #define COMMAND_TABLE_INCREMENT 5

 typedef struct command_table_elem {
 	char * name;
 	command * cmd;
 } command_table_elem;

 typedef struct command_table {
 	int size;
 	int alloc;
 	command_table_elem * tbl;
 } command_table;

///Create a new runtime table.
command_table * command_table_new();

 ///Add to a runtime table.
 error * command_table_add(command_table * tbl, char * name, command * cmd);

#endif
