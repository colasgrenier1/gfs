#include <string>

#include "gfs.hpp"

/**
 * The text class
 */
class Text : public Element {
	std::string content;
public:
	Text(int line, int col): Element("text", line, col, true, false, false) {

	}

};

/**
 * Initialize the text elements upon the document.
 */
void initializeText(Document & doc) {

};
