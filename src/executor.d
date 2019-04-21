import std.container: DList;

import document;
import parser;

/**
 *
 */
interface Command {
	void execute(Document document, Context context, DList!Token tokens);
}

/**
 *
 */
class Frame {
	string name;
	Element element;
	Command[string] commands;
	//Determines if we can pop this from the stack from another element
	bool canPop;
	//Determines if we can have text
	bool textContainer;
	//Determines if the element can have paragraphs
	bool paragraphContainer;
	//Used to determine when new paragraphs are required
	bool newParagraphMayBeRequired;
	bool newParagraphRequired;

	this(string name) {
		this.name = name;
	}
}

/**
 *
 */
class Context {
	private DList!Frame frames;
	private Frame toplevel;

	this() {
		frames = DList!Frame();
		toplevel = new Frame("toplevel");
		frames.insert(toplevel);
	}

	/**
	 * Returns the current frame.
	 * If no frames left, returns the toplevel.
	 */
	Frame current() {
		return frames.back();
	}

	/**
	 * Adds a new frame.
	 */
	void add(string name) {
		frames.insert(new Frame(name));
	}
	void add(string name, Element element, bool canPop, bool paragraphContainer) {
		auto f = new Frame(name);
		f.element = element;
		f.canPop = canPop;
		f.paragraphContainer = paragraphContainer;
		frames.insert(f);
	}

	/**
	 * Remove the current frame
	 */
	void pop() {

	}

	/**
	 * Returns the command.
	 */
	Command resolve(string name) {
		//tmps:
		foreach_reverse (f; frames) {
			Command cmd = f.commands.get(name, null);
			if (cmd !is null) {
				return cmd;
			}
		}
		throw new Exception("Command not found: '" ~ name ~ "'.");
	}

	/**
	 * Sets a command in the local context.
	 */
	void register(string name, Command command) {

	}

	/**
	 * Sets a global command.
	 *
	 * NOTE: this will not cancel local definitions,
	 *       only it allows definitions to apply out of
	 *       their scope of declaration.
	 */
	void registerGlobal(string name, Command command) {

	}
}

/**
 * Creates a new paragraph
 */
void newParagraph(Document document, Context context) {

}

/**
 * Seeks the nearest text container or creates a paragraph if it encounters
 * something with containsParagraphs=true.
 */
void seekTextContainer(Document document, Context context) {
	//Go to the nearest text or paragraph container
	while (!(context.current().textContainer || context.current().paragraphContainer)) {
		if (!context.current().canPop) {
			throw new Exception("Encountered non-poppable element in quest of");
		}
		context.pop();
	}
	//We determine which came: text or p container
	//if not t container we know this is a pcontainer
	//and that thus we need to append a paragraph
	if (!context.current().textContainer) {
		document.push(new Paragraph());
	}
}

/**
 * Execute the batch of tokens
 */
void execute(Document document, Context context, DList!Token tokens) {
	while (!tokens.empty()) {
		//We take an element
		auto tok = tokens.back();
		tokens.removeBack();//remove

		//Depending on the token type
		if (tok.type == TokenType.EMPTY) {
			//We pass
		} else if (tok.type == TokenType.NEWLINE) {
			//We either will add a space or we put paragraph needed
			//The space will be added later?
			if (context.current().newParagraphMayBeRequired) {
				context.current().newParagraphMayBeRequired = false;
				context.current().newParagraphRequired = true;
			} else {
				context.current().newParagraphMayBeRequired = true;
			}
		} else if (tok.type == TokenType.SPACE) {

		} else if (tok.type == TokenType.WORD) {
			//We start by setting that a paragraph is not required
			if (context.current().newParagraphMayBeRequired) {
				context.current().newParagraphMayBeRequired = false;
			}
			//If a paragraph is required we need to do it.
			//Need to backtrack until the paragraph
			if (context.current().newParagraphRequired) {
				newParagraph(document, context);
			}
			//We add to the current paragraph
		} else if (tok.type == TokenType.COMMAND) {
			context.resolve(tok.content).execute(document, context, tokens);
		} else if (tok.type == TokenType.BLOCK) {
			context.add("block");
		}
	}
}
