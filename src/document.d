import std.container: DList;

/**
 *
 */
class Element {
	DList!Element children;

	void addChild(Element child) {
		children.insert(child);
	}

	DList!Element getChildren() {
		return children;
	}
}

/**
 * A Document is the toplevel object.
 */
class Document {

	//The stack of elements
	DList!Element stack;

	//Constructor
	this() {
		stack = DList!Element();
	}

	void push(Element elem) {
		stack.insert(elem);
	}

	Element pop() {
		auto tmp = stack.back();
		stack.removeBack();
		return tmp;
	}

}

/**
 *
 */
class Section : Element {

}

/**
 *
 */
class Paragraph : Element {

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
