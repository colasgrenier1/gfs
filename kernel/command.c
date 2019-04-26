#include <string.h>

#include "command.h"
#include "document.h"
#include "parser.h"

error * command_execute(command * cmd, document * doc, token_list * tokens) {
	return cmd->execute(doc, tokens, cmd->private);
}

command * command_new(error* (*execute)(document * doc, token_list * tok, void * private), void * private, void (*free)(void * private)) {
	command * c = malloc(sizeof(command));
	c->execute = execute;
	c->private = private;
	c->free = free;
	return c;
}

command_table * command_table_new() {
	command_table * tbl = malloc(sizeof(command_table));
	tbl->size = 0;
	tbl->alloc = COMMAND_TABLE_DEFAULT_SIZE;
	tbl->tbl = malloc(sizeof(command_table_elem)*COMMAND_TABLE_DEFAULT_SIZE);
	return tbl;
}

error * command_table_add(command_table * tbl, char * name, command * cmd) {
	//we add space if there is none
	if (tbl->size >= tbl->alloc) {
		command_table_elem * old = tbl->tbl;
		tbl->tbl = malloc(sizeof(command_table_elem)*(tbl->alloc+COMMAND_TABLE_INCREMENT));
		memcpy(tbl->tbl,old,tbl->size*sizeof(command_table_elem));
		free(old);
	}
	//We add te element
	tbl->tbl[tbl->size].name = name;
	tbl->tbl[tbl->size].cmd = cmd;
	tbl->size += 1;
	return NULL;
}
