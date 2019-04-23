import std.container: DList;

/**
 * A document element.
 */
class Element {
	///A list of children of this element.
	private DList!Element children;

	///Determines if this element can be popped in searched of another
	bool canPop = false;

	///Determines if this element can contain text.
	bool textContainer = false;
	///Determines if this element can contain paragraphs.
	bool paragraphContainer = false;

	//Used to determine when new paragraphs are required
	bool newParagraphMayBeRequired = false;
	bool newParagraphRequired = false;

	this () {

	}

	void addChild(Element child) {
		children.insert(child);
	}

	DList!Element getChildren() {
		return children;
	}
}

/**
 * A Document is the toplevel object.
 * It is also an element.
 */
class Document : Element {

	//The stack of elements
	private DList!Element stack;

	//Constructor
	this() {
		super();
		stack = DList!Element();
		paragraphContainer = true;
	}

	void push(Element elem) {
		stack.insert(elem);
	}

	Element pop() {
		auto tmp = stack.back();
		stack.removeBack();
		return tmp;
	}

	Element current() {
		return stack.back();
	}
}

/**
 *
 */
class Section : Element {

}

/**
 * This really is the base for all other element: text, italic, etc.
 * attach upon a paragraph.
 */
class Paragraph : Element {
	this () {
		super();
	}

}

/**
 *
 */
class Text : Element {

}

/**
 *
 */
class FootNote : Element {

}

/**
 *
 */
class URL : Element {

}

/**
 *
 */
class BlockQuote : Element {

}

/**
 *
 */
class Table : Element {

}

/**
 *
 */
class Plot : Element {

}

/**
 *
 */
class Image : Element {
		string source;
		string caption;
}
