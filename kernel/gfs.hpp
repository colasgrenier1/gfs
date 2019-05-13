#ifndef GFSHEADER
#define GFSHEADER

#include <ctime>
#include <cstdarg>
#include <list>
#include <iostream>
#include <map>
#include <stack>
#include <string>

#include <boost/format.hpp>

/*******************************************************************************
 *                                                                             *
 *                                 E R R O R                                   *
 *                                                                             *
 ******************************************************************************/

class Error : public std::exception {
private:
	char buf[4096];
	std::string msg;
public:
	Error(std::string const & message) {
		msg = message;
	}
	Error(char const * fmt, ...) {
		va_list l;
		va_start(l, fmt);
		vsnprintf(buf, sizeof buf, fmt, l);
		va_end(l);
		std::string s(buf);
	}
	~Error() throw () {}
	const char * what() const throw() {
		return msg.c_str();
	}
};

/*******************************************************************************
 *                                                                             *
 *                  F O R W A R D   D E C L A R A T I O N S                    *
 *                                                                             *
 ******************************************************************************/

class Command;
class Document;
class Renderer;

/*******************************************************************************
 *                                                                             *
 *                               P A R S I N G                                 *
 *                                                                             *
 ******************************************************************************/

enum TokenType {
	EMPTY   = 0,
	NEWLINE = 1,
	SPACE   = 2,
	WORD    = 3,
	COMMAND = 4,
	BLOCK   = 5
};

class Token { public:
	TokenType type;
	int line;
	int column;
	///String content
	std::string content;
	///Token content for blocks
	std::list<Token> contents;
	///Whether the command (if it is a command token!) has finished by a
	///semicolon escape.
	bool broken;
	//Constructors
	Token(TokenType type, int line, int col);
	Token(TokenType type, int line, int col, char c);
	Token(TokenType type, int line, int col, std::string s);
	Token(TokenType type, int line, int col, std::list<Token> children);
};

/**
 * Get a list of tokens from a stream.
 */
std::list<Token> * parse(std::istream & in);
std::list<Token> * parse(std::istream & in, int& line, int&col, bool inblock);

/**
 * Write the tokens to file, one per line, listing their type & indenting.
 */
void dump_tokens(std::list<Token> * tokens, std::ostream & out);
void dump_tokens(std::list<Token> * tokens, std::ostream & out, int level);

/**
 * Get the raw representation of a list of tokens, including spaces & newlines
 * that are expanded.
 */
std::string raw(std::list<Token> list);

/*******************************************************************************
 *                                                                             *
 *                              E L E M E N T S                                *
 *                                                                             *
 ******************************************************************************/

/**
 * The control block for an element. Contains renderers, options, etc.
 */
class ElementControlBlock {
public:
	std::string name;
	std::list<Renderer*> renderers;
	virtual void setOption(std::string & option, std::string & content) = 0;
};

/**
 * A document element node.
 */
class Element {
public:
	/**
	 * The line an column of the token which started this element.
	 */
	int line;
	int column;

	/**
	 * The name of the type of this element.
	 */
	std::string name;

	/**
	 * Wether or not this element may be popped in search of a new one.
	 */
	bool canPop;

	/**
	 * Whether or not this element can contain text or paragraphs.
	 */
	bool textContainer;
	bool paragraphContainer;

	/**
	 * The children of this element.
	 */
	std::list<Element*> children;

	/**
	 * Child operations.
	 */
	void addChild(Element * child);
	bool hasChildren();
	void removeChild(int n);
	int countChildren();
	Element * lastChild();
	Element * getChild(int n);
	std::list<Element*> getChildren();

	/**
	 * Dump a debug represenation of this element.
	 */
	void dump(std::ostream & out);
	void dump(std::ostream & out, int indent);

	/**
	 * Constructors & Destructors.
	 */
	Element(std::string n, int l, int c, bool cp, bool tc, bool pc);
	virtual ~Element();


};

class Renderer {
public:
	virtual void render(Element * elem, std::ostream & out);
	virtual void setOption(std::string & option, std::string & content);
};

/*******************************************************************************
 *                                                                             *
 *                               C O M M A N D                                 *
 *                                                                             *
 ******************************************************************************/

/**
 * Command object.
 */
class Command {
	virtual void execute(Document document, std::list<Token> tokens) = 0;
};

/**
 * Token list manipulation helper functions.
 * These will be useful for writing commands.
 */
 ///Get the next word in the input stream. Skips spaces, fails on newlines,
 ///commands and blocks.
std::string getNextWord(std::list<Token> tokens);
///Skip until the next word or command, fails on newline.
void skipSpace(std::list<Token> tokens);
///Returns all the tokens until end of line (not that newline occurying in
///blocks started after this will not count, the newline will be the first one
///on the same level as the current token)
std::list<Token> getTextToEOL(std::list<Token> tokens);
///Gets the next block, skipping spaces, failing if it encounters commands, words,
///newlines.
std::list<Token> getNextBlock(std::list<Token> tokens);
///Get the next blocks, skipping spaces and failing on newline, word, command.
std::list<std::list<Token> > getNextBlocks(std::list<Token> tokens, int n);

/*******************************************************************************
 *                                                                             *
 *                              D O C U M E N T                                *
 *                                                                             *
 ******************************************************************************/

/**
 * The toplevel element used.
 */
class DocumentElement : public Element {

};

/**
 * Document object.
 */
class Document {
private:

	class Frame {
		std::map<std::string, Command*> commands;
		std::map<std::string, std::string> variables;
	};

	std::map<std::string, ElementControlBlock*> elements;
	std::list<Element *> elementStack;
	std::list<Frame> frameStack;

public:
	/**
	 *
	 */
	Document();

	/**
	 * Encoding of the document.
	 */
	///Input encoding of the text.
	std::string inputEncoding;
	///Output encoding.
	std::string outputEncoding;

	/**
	 * Registration of element control blocks and renderers, and options.
	 */
	void registerElement(ElementControlBlock * ecb, std::string name);
	void registerElementRenderer(std::string element, std::string format, Renderer r);
	void setElementOption(std::string element, std::string option, std::string content);
	void setRendererOption(std::string element, std::string format, std::string option, std::string content);

	/**
	 * Document stack maintenance.
	 *
	 * NOTE: These must only be used with certainty, as they do not perform
	 *       checks. It is preferrable to use the seek* functions provided
	 *       below.
	 */
	void push(Element * elem);
	Element * current();
	Element * pop();

	/**
	 * Document variable/command context frames.
	 */
	///Enter a new frame
	void enter();
	///Leave the current frame.
	void leave();
	///Register a command.
	void setCommand(Command * cmd, std::string name);
	 ///Resolves a command.
	Command * getCommand(std::string name);
	///Unset a command.
	void unsetCommand(std::string name);
	///set a variable
	void setVariable(std::string variable, std::string name);
	///get a variable
	std::string getVariable(std::string name);
	///unset a variable
	void unsetVariable(std::string name);

	/**
	 *
	 */
	void seekElement(Element elem);
	void seekElementType(std::string type);
	void seekParagraph();
	void seekParagraphContainer();
	void seekText();
	void seekTextContainer();

	/**
	 *
	 */
	void newParagraph();

	/**
	 * Execute on these tokens.
	 */
	void execute(std::list<Token> * tokens);

};

/**
 * Load the builtin elements into the document.
 */
void loadBuiltin(Document doc);

/*******************************************************************************
 *                                                                             *
 *                               R U N T I M E                                 *
 *                                                                             *
 ******************************************************************************/

class Extension {
public:
	std::string path;
	time_t loaded;
	void * ptr;
	Extension(std::string location);
	~Extension();
};

class Runtime {
public:
	void loadPlugin(std::string path);
	void loadDirectory(std::string dir);
	Document newDocument();
};

#endif
