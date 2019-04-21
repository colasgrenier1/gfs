/**
 *
 *
 */

import std.stdio;

import document;
import executor;
import parser;

/**
 *
 */
void main() {

	//We get the tokens
	auto t = parse(File("test.gfs"));

	dump_tokens(t);


	//We instantiate a context
	auto c = new Context();
	auto d = new Document();

	//We execute
	execute(d, c, t);

}
