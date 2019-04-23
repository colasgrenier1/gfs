import document

/**
 *
 */
interface Command {

}


/**
 * The Context keeps track of variables, commands, etc.
 */
class Context {

	/**
	 * The frame contains locals for on context.
	 */
	private class Frame {
		/**
		 *
		 */

		private HashMap<String, Command> commands

		Frame() {
			commands = new HashMap<String, Command>()
		}

		void register(Command cmd, String name) {
			commands.set(name, cmd)
		}

		boolean contains(String name) {
			return commands.containsKey()
		}

		boolean get(String name) {
			return commands.get(name)
		}

	}

	/**
	 *
	 */
	private LinkedList<Frame> stack

	Context() {
		stack = new LinkedList<Frame>()
		stack.add(new Frame())
	}

	/**
	 * Enter a block.
	 */
	void push() {
		stack.add(new Frame())
	}

	/**
	 * Exit a block.
	 */
	void pop() {
		stack.removeLast()
	}

	/**
	 * Tries to find the command.
	 */
	Command resolve(String name) {
		for (frame in stack.descendingIterator()) {
			if (frame.contains(name)) {
				return frame.get(name)
			}
		}
		throw new Error("Unknown command! " + name)
	}

	/**
	 * Sets the command in the local context.
	 */
	void Register(Command cmd, String name) {
		stack.top().register(cmd, name)
	}

}

void execute(Document document, Context context, LinkedList<Token> tokens) {

}
