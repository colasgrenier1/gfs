#include "command.h"
#include "document.h"
#include "parser.h"

error * command_execute(command * cmd, document * doc, token_list * tokens) {
	return cmd->execute(cmd, doc, tokens);
}
