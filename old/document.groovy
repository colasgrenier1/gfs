class Element {
	
}

/**
 *
 */
class Document {
	List content
	Document () {
		content = LinkedList<Element>()
	}
}


/**
 * A generalised section to be used by users (carries no
 * programming content beyond being used by others).
 */
class Section {

}

/**
 *
 */
class Paragraph {
	List content
	Paragraph () {
		content = new ArrayList()
	}
}

/**
 * A line is like a paragraph except it does not allow
 * multiple ones: if a new paragraph is detected in the
 * context an error is thrown.
 */
class Line {

}

/**
 * A single word, without punctuation or space.
 */
class Word implements TextElement {
	String content
	Word (String content) {
		this.content = content
	}
}

/**
 * Space.
 */
class Space implements TextElement {
	boolean breaking
	Word (boolean breaking) {
		this.breaking = breaking
	}
	Word () {
		breaking = false
	}
}

/**
 * Punctuation mark.
 */
class Punctuation implements TextElement {
	String content
	Punctuation (String content) {
		this.content = content
	}
}

interface TextElement {
	//I think this will be left empty for now
}

/**
 *
 */
class Text {
	List content
	Text () {
		content = ArrayList()
	}

	void add(Object elem) {

	}
}

/**
 *
 */
class Footnote {

}

/**
 *
 */
class BlockQuote {

}

/**
 *
 */
class Poetry {

}

/**
 *
 */
class URL {

}

interface Element {

}
