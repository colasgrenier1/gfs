/*
 *  SFS Lexer
 *
 *
 */

import java.util.LinkedList;
import java.util.List;
import groovy.transform.InheritConstructors

private void parser_dbg (String str) {
	println str
}

public enum TokenType {
	EMPTY,
	NEWLINE,
	SPACE,
	WORD,
	COMMAND,
	BLOCK
}

public class Token {
	public int lineno
	public TokenType type
	public Object content
	Token(TokenType type, int lineno, Object content) {
		this.type = type
		this.lineno = lineno
		this.content = content
	}
}

@InheritConstructors
class ParserError extends Exception {
}

/**
 * Reads from a file and returns a list of tokens.
 * @param stream a stream to read from
 * @param fail_on_eof determines if it fails on EOF (nested blocks must end by > not by EOF, for example)
 */
public LinkedList<Token> parse(BufferedReader stream, int lineno = 1, boolean fail_on_eof = false) {
	int cv
	String c
	LinkedList<Token> list = new LinkedList<Token>()
	while ((cv=stream.read())!=-1) {
		c = (String)((char) cv)
		parser_dbg "Read " + c
		if (c == "\n" || c == "\r") {
			parser_dbg "newline"
			if (!list.isEmpty() && list.peekLast().type == TokenType.NEWLINE) {
				list.peekLast().content += c
			} else {
				list.add(new Token(TokenType.NEWLINE, lineno, c))
			}
			//We increment lineno on "\n"
			if (c == "\n") {
				lineno += 1
			}
		} else if (c == " " || c == "\t") {
			if (!list.isEmpty() && list.peekLast().type == TokenType.SPACE) {
				list.peekLast().content += c
			} else {
				list.add(new Token(TokenType.SPACE, lineno, c))
			}
		} else if (c == "@") {
			//Two @@ equals one character @
			if (!list.isEmpty() && list.peekLast().type == TokenType.COMMAND && list.peekLast().content.length() == 0) {
				//We remove the command element
				list.pop()
				//We append the @ character to a string otherwise we create
				if (!list.isEmpty() && list.peekLast().type == TokenType.WORD) {
					list.peekLast().content += c
				} else {
					list.add(new Token(TokenType.WORD, lineno, c))
				}
			} else {
				//We create a command
				list.add(new Token(TokenType.COMMAND, lineno, ""))
			}
		} else if (c == "<") {
			//@< is an escape for a literal < character
			if (!list.isEmpty() && list.peekLast().type == TokenType.COMMAND && list.peekLast().content.length() == 0) {
				//We remove the command element
				list.pop()
				//If previosu element is a word we append otherwise we create
				if (!list.isEmpty() && list.peekLast().type == TokenType.WORD) {
					list.peekLast().content += c
				} else {
					list.add(new Token(TokenType.WORD, lineno, c))
				}
			} else {
				//This is not a literal but the start of a block
				//We add the block and parse it (block ended by another >)
				//(note that we put fail_on_eof=True, because this block cannot
				//end of EOF, but must end with >)
				list.add(new Token(TokenType.BLOCK, lineno, parse(stream, lineno, true)))
			}
		} else if (c == ">") {
			//@> is an escape for a literal ; character
			if (!list.isEmpty() && list.peekLast().type == TokenType.COMMAND && list.peekLast().content.length() == 0) {
				//We remove the command element
				list.pop()
				//If the previous is a word we append to it ortherwise we create
				if (!list.isEmpty() && list.peekLast().type == TokenType.WORD) {
					list.peekLast().content += c
				} else{
					list.add(new Token(TokenType.WORD, lineno, c))
				}
			} else {
				//This signals the end of the block
				//we return the list
				return list;
			}
		} else if (c == ";") {
			//; can be used to terminate a command inline
			if (!list.isEmpty() && list.peekLast().type == TokenType.COMMAND) {
				//We terminate the command by placing a EMPTY token,
				//which will prevent the next character from being appended
				//to the command name, etc.
				list.add(new Token(TokenType.EMPTY, lineno, null))
			} else {
				if(!list.isEmpty() && list.peekLast().type == TokenType.WORD) {
					list.peekLast().content += c
				} else {
					list.append(new Token(TokenType.WORD, lineno, c))
				}
			}
		} else {
			//This is a random character, we place it in a word or in a command
			if (!list.isEmpty() && (list.peekLast().type == TokenType.COMMAND || list.peekLast().type == TokenType.WORD)) {
				list.peekLast().content += c
			} else {
				list.add(new Token(TokenType.WORD, lineno, c))
			}
		}
	}

	//We are at the end of stream
	if (fail_on_eof) {
		throw ParserError("Filed ended : expecting >")
	}

	return list

}


/**
 * Write to stdout to debug.
 */
void dump(List<Token> list) {

	for (Token tok : list) {
		switch(tok.type) {
			case TokenType.WORD:
				println "WORD  " + tok.content
				break
			case TokenType.COMMAND:
				println "CMD   " + tok.content
				break
			case TokenType.SPACE:
				println "SPACE "
				break
			case TokenType.BLOCK:
				println "BLOCK " + tok.content.size().toString()
				dump(tok.content)
				break
			case TokenType.NEWLINE:
				println "NEWL"
				break
		}
	}
}

class A {
static void main (String[] args) {
//We will test this
def l = parse(new BufferedReader(new FileReader("test.gfs")))
dump(l)
}
}
