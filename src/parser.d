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
		this.content = content;
	}

	this(TokenType type, DList!Token contents) {
		this.type = type;
		this.contents = contents;
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

/*
 * Dump tokens to stdout.
 */
void dump_tokens(DList!Token tokens) {
	_dump_tokens(tokens, 2);
}

private void _dump_tokens(DList!Token tokens, int level) {
	foreach (Token tok; tokens) {
		for (int i=0; i<level; i++) {
			write(" ");
		}
		switch (tok.type) {
			case TokenType.EMPTY:
				writeln("EMPTY");
				break;
			case TokenType.NEWLINE:
				writeln("NEWLINE");
				break;
			case TokenType.SPACE:
				writefln("%-10s '%s'", "SPACE", tok.content);
				break;
			case TokenType.WORD:
				writefln("%-10s %s", "WORD", tok.content);
				break;
			case TokenType.COMMAND:
				writefln("%-10s %s", "COMMAND", tok.content);
				break;
			case TokenType.BLOCK:
				writefln("%-10s", "BLOCK");
				_dump_tokens(tok.contents, level+2);
				break;
			default:
				writeln("OTHER!!!!");
		}
	}
}
