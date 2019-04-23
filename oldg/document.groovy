/**
 * The base class for all document elements (including the document itself!).
 */
class Element {

	/**
	 * This is the name of the element.
	 */
	String name

	/**
	 * Determines if this element can be removed from the stack in search of
	 * another
	 */
	 boolean canPop

	 /**
	  * Determines if text can be placed in it as children.
	  */
	 boolean textContainer

	 /**
	  * Determines if paragraphs can be children.
	  */
	 boolean paragraphContainer

}

/**
 *
 */
class Document extends Element {



	/**
	 * This stack is used to keep track of the current elements.
	 */
	private Stack<Element> stack

	/**
	 * Registers an element on the document stack.
	 */
	void push(Element elem) {
		stack.push(elem)
	}

	/**
	 * Returns the current element on the document stack.
	 */
	Element current() {
		return stack.top()
	}

	/**
	 *
	 */
	Element pop() {
		return stack.pop()
	}

}

/**
 *
 */
class Paragraph extends Element {

}
