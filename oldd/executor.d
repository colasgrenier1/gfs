import std.container: DList;
import std.stdio: writeln, writefln;

import document;
import parser;

/**
 * The runtime contains all external object definitions and renderers.
 */
class RunTime {

}


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
	Command[string] commands;

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
	//Go to the nearest paragraph container
	while (!(document.current().paragraphContainer)) {
		if (!(document.current().canPop)) {
			throw new Exception("Encountered non-poppable element in quest of pc");
		}
		document.pop();
	}
	//We are at the paragraph container
	Paragraph p = new Paragraph();
	document.current().addChild(p);
	document.push(p);
}

/**
 * Seeks the nearest text container or creates a paragraph if it encounters
 * something with containsParagraphs=true.
 */
void seekTextContainer(Document document, Context context) {
	//Go to the nearest text or paragraph container
	while (!(document.current().textContainer || document.current().paragraphContainer)) {
		if (!document.current().canPop) {
			throw new Exception("Encountered non-poppable element in quest of");
		}
		document.pop();
	}
	//We determine which came: text or p container
	//if not t container we know this is a pcontainer
	//and that thus we need to append a paragraph
	if (!document.current().textContainer) {
		document.push(new Paragraph());
	}
}

/**
 * Execute the batch of tokens
 */
void execute(Document document, Context context, DList!Token tokens) {
	while (!tokens.empty()) {
		//We take an element
		auto tok = tokens.front();
		tokens.removeFront();//remove

		//Depending on the token type
		if (tok.type == TokenType.EMPTY) {
			writefln("PROCESSING EMPTY");
			//We pass
		} else if (tok.type == TokenType.NEWLINE) {
			writefln("PROCESSING NEWLINE");
			//We either will add a space or we put paragraph needed
			//The space will be added later?
			if (document.newParagraphMayBeRequired) {
				document.newParagraphMayBeRequired = false;
				document.newParagraphRequired = true;
			} else {
				document.newParagraphMayBeRequired = true;
			}
		} else if (tok.type == TokenType.SPACE) {
			writefln("PROCESSING SPACE '%s'", tok.content);
		} else if (tok.type == TokenType.WORD) {
			writefln("PROCESSING WORD '%s'", tok.content);
			//We start by setting that a paragraph is not required
			if (document.newParagraphMayBeRequired) {
				document.newParagraphMayBeRequired = false;
			}
			//If a paragraph is required we need to do it.
			//Need to backtrack until the paragraph
			if (document.newParagraphRequired) {
				newParagraph(document, context);
			}
			//We add to the current paragraph
		} else if (tok.type == TokenType.COMMAND) {
			writefln("PROCESSING COMMAND '%s'", tok.content);
			context.resolve(tok.content).execute(document, context, tokens);
		} else if (tok.type == TokenType.BLOCK) {
			writefln("PROCESSING BLOCK");
			context.add("block");
		}
	}
}
