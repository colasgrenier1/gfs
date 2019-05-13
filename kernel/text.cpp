#include <string>

#include "gfs.h"

class Word {
public:
	std::string content;
	Word() {

	}
	Word(std::string con) {
		content = con;
	}
	Word(char c) {
		content.append(&c, 1);
	}
}

class Space {
	bool breaking;
	int length;
}

enum PunctuationType {
	//Intra-"word"
	APOSTROPHE,
	HYPHEN,

	//Inter-word
	DASH,

	//QUOTING
	OPEN_QUOTE,
	CLOSE_QUOTE,

	//Punctuation per-se
	PERIOD,
	COMMA,
	COLON,
	SEMICOLON,
	EXCLAMATION,
	QUESTION
};

class Punctuation {

};

/**
 *
 */
class TextElementControlBlock : public ElementControlBlock {

};

/**
 * The text class
 */
class Text : public Element {
	std::list<>
public:
	Text(int line, int col): Element(line, col) {

	}

};

/**
 * Initialize the text elements upon the document.
 */
void initializeText(Document & doc) {

};
