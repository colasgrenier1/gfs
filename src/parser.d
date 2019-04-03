import std.container : DList;
import std.conv: to;
import std.stdio;

enum TokenType {
	EMPTY = 0,
	NEWLINE = 1,
	SPACE = 2,
	WORD = 3,
	COMMAND = 4,
	BLOCK = 5
}

class Token {
	TokenType type;

	union {
		string content;
		DList!Token contents;
	}

	this(TokenType type, string content) {
		this.type = type;
	}

	this(TokenType type, DList!Token contents) {

	}

}

DList!Token parse(File f) {
	DList!Token tokens;
	string c;
	char[1] ca;
	while (!f.eof()) {
		if ((f.rawRead(ca)).length==0) {
			return tokens;
		}
		c = to!string(ca[0]);
		writeln("PARSING CHARACTER '" ~ c ~ "'.");
		if (c=="\n") {
			if (!tokens.empty() && tokens.back().type==TokenType.NEWLINE && tokens.back().content=="\r") {
				tokens.back().content ~= c;
			} else {
				tokens.insert(new Token(TokenType.NEWLINE, to!string(c)));
			}
		} else if (c=="\r") {
			tokens.insert(new Token(TokenType.NEWLINE, to!string(c)));
		} else if (c==" " || c=="\t") {
			if (!tokens.empty() && tokens.back().type==TokenType.SPACE) {
				//tokens.back().content ~= to!string(c);
				tokens.back().content ~= c;
			} else {
				tokens.insert(new Token(TokenType.SPACE, to!string(c)));
			}
		} else if (c=="@") {
			//If there is a
			if (!tokens.empty() && tokens.back().type==TokenType.COMMAND && tokens.back().content=="") {
				tokens.back().content ~= c;
			} else {
				tokens.insert(new Token(TokenType.COMMAND, ""));
			}
		} else if (c=="<") {
			if (!tokens.empty() && tokens.back().type==TokenType.COMMAND && tokens.back().content=="") {
				//If there is a @ before this is escaped
				tokens.removeBack();
				if (!tokens.empty() && tokens.back().type==TokenType.WORD) {
					tokens.back().content ~= c;
				} else {
					tokens.insert(new Token(TokenType.WORD, c));
				}
			} else {
				//Otherwise we start a block
				tokens.insert(new Token(TokenType.BLOCK, parse(f)));
			}
		} else if (c==">") {
			if (!tokens.empty() && tokens.back().type==TokenType.COMMAND && tokens.back().content=="") {
				//If there is a @ before this is escaped
				tokens.removeBack();
				if (!tokens.empty() && tokens.back().type==TokenType.WORD) {
					tokens.back().content ~= c;
				} else {
					tokens.insert(new Token(TokenType.WORD, c));
				}
			} else {
				//This symbolises the end of this
				return tokens;
			}
		} else if (c==";") {
			if (!tokens.empty() && tokens.back().type==TokenType.COMMAND) {
				tokens.insert(new Token(TokenType.EMPTY, ";"));
			} else {
				if (!tokens.empty() && tokens.back().type==TokenType.WORD) {
					tokens.back().content ~= c;
				} else {
					tokens.insert(new Token(TokenType.WORD, c));
				}
			}
		} else {
			if (!tokens.empty() && (tokens.back().type==TokenType.WORD || tokens.back().type==TokenType.COMMAND)) {
				tokens.back().content ~= c;
			} else {
				tokens.insert(new Token(TokenType.WORD, c));
			}
		}
	}
	return tokens;
}
