/**
 * A command executes on an object.
 */
class Command {

}


/**
 *
 */
class ContextElement {
	Map<String. Command> commands
	Object object

}

/**
 * The context contains a stack of elements and of
 * command definitions (which are contained within
 * the definition of the element?).
 */
class Context {

	/**
	 * Returns the command object.
	 */
	Command resolve(String name) {

	}

}

/**
 * Execute the tokens on the current document.
 */
void execute(Context context, List<Token> tokens) {

	//First we check if there is a paragraph in the context
	if context.first().content.


}
