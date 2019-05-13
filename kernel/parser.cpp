#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/format.hpp>

#include "gfs.hpp"

Token::Token(TokenType ptype, int pline, int pcol) {
	type = ptype;
	line = pline;
	column = pcol;
}

Token::Token(TokenType ptype, int pline, int pcol, char c) {
	type = ptype;
	line = pline;
	column = pcol;
	content.append(&c, 1);
}

Token::Token(TokenType ptype, int pline, int pcol, std::string s) {
	type = ptype;
	line = pline;
	column = pcol;
	content = s;
}

Token::Token(TokenType ptype, int pline, int pcol, std::list<Token> children) {
	type = ptype;
	line = pline;
	column = pcol;
	contents = children;
}

std::list<Token> * parse(std::istream & in) {
	int line=1;
	int col=1;
	return parse(in, line, col, false);
}

std::list<Token> * parse(std::istream & in, int& line, int&col, bool inblock) {
	std::list<Token> * tokens = new std::list<Token>();
	char c;
	while (in.get(c)) {
		//std::cout << c << std::endl;
		if (c=='\n' || c=='\r') {
			if (!tokens->empty() && tokens->back().type==NEWLINE) {
				tokens->back().content.append(&c, 1);
			} else {
				tokens->push_back(Token(NEWLINE, line, col, c));
				line += 1;
				col = 1;
			}
		} else if (c==' ' || c=='\t') {
			col + 1;
			if (!tokens->empty() && tokens->back().type==SPACE) {
				tokens->back().content.append(&c, 1);
			} else {
				tokens->push_back(Token(SPACE, line, col, c));
			}
		} else if (c=='<') {
			col += 1;
			if (!tokens->empty() && tokens->back().type==COMMAND && tokens->back().content.length()==0) {
				tokens->pop_back();
				if (!tokens->empty() && tokens->back().type==WORD) {
					tokens->back().content.append("<");
				} else {
					tokens->push_back(Token(WORD, line, col, "<"));
				}
			} else {
				tokens->push_back(Token(BLOCK, line, col, *parse(in, line, col, true)));
			}
		} else if (c=='>') {
			col += 1;
			if (!tokens->empty() && tokens->back().type==COMMAND && tokens->back().content.length()==0) {
				tokens->pop_back();
				if (!tokens->empty() && tokens->back().type==WORD) {
					tokens->back().content.append(">");
				} else {
					tokens->push_back(Token(WORD, line, col, ">"));
				}
			} else {
				if (!inblock) {
					std::cout << "reached < error" << std::endl;
					throw Error(boost::str(boost::format("Reached > while expecting EOF at line %d column %d!") % line % col ));
				}
				return tokens;
			}
		} else if (c=='@') {
			col += 1;
			if (!tokens->empty() && tokens->back().type==COMMAND && tokens->back().content.length()==0) {
				tokens->pop_back();
				if (!tokens->empty() && tokens->back().type==WORD) {
					tokens->back().content.append("@");
				} else {
					tokens->push_back(Token(WORD, line, col, "@"));
				}
			} else {
				tokens->push_back(Token(COMMAND, line, col, ""));
			}
		} else if (c==';') {
			col += 1;
			if (!tokens->empty() && tokens->back().type==COMMAND) {
				tokens->push_back(Token(EMPTY, line, col));
			} else {
				if (!tokens->empty() && tokens->back().type==WORD) {
					tokens->back().content.append(";");
				} else {
					tokens->push_back(Token(WORD, line, col, ";"));
				}
			}
		} else {
			col += 1;
			if (!tokens->empty() && (tokens->back().type==WORD || tokens->back().type==COMMAND)) {
				tokens->back().content.append(&c, 1);
			} else {
				tokens->push_back(Token(WORD, line, col, c));
			}
		}
	}

	//We have reached EOF
	if (inblock) {
		throw Error("Reached end of file while still in block!");
	}

	return tokens;
}

void dump_tokens(std::list<Token> * tokens, std::ostream & out) {
	dump_tokens(tokens, out, 0);
}

void dump_tokens(std::list<Token> * tokens, std::ostream & out, int level) {
	for (Token t: *tokens) {
		std::cout << std::setw(7);
		std::cout << t.line;
		std::cout << std::setw(7);
		std::cout << t.column;
		std::cout << "   ";
		for (int i = 0; i < level; i++) {
			out << " ";
		}
		switch (t.type) {
			case EMPTY:
				out << "EMPTY" << std::endl;
				break;
			case NEWLINE:
				out << "NEWLINE" << std::endl;
				break;
			case SPACE:
				out << "SPACE" << std::endl;
				break;
			case WORD:
				out << "WORD    \'" << t.content << "\'" << std::endl;
				break;
			case COMMAND:
				out << "COMMAND \'" << t.content << "\'" << std::endl;
				break;
			case BLOCK:
				out << "BLOCK" << std::endl;
				dump_tokens(&t.contents, out, level+4);
				break;
			default:
				out << "?????" << std::endl;
				break;
		}
	}
}

int main(int argc, char ** argv) {
	std::list<Token> * tok;
	std::ifstream in;
	in.open(argv[1]);
	try {
		tok = parse(in);
	} catch (const Error & e) {
		std::cout << "error!" << std::endl;
		std::cout << e.what() << std::endl;
		return -1;
	} catch (const std::exception & e) {
		std::cout << "unknown error" << std::endl;
		return -1;
	}
	//dump_tokens(tok, std::cout);
	return 0;
}
