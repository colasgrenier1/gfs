#include <map>

#include "gfs.hpp"

Document::Document() {

}

void Document::registerElement(ElementControlBlock * ecb, std::string name) {
	elements[name] = ecb;
}

void Document::registerElementRenderer(std::string element, std::string format, Renderer * r) {
	ElementControlBlock * ecb = elements[element];
	ecb->renderers[format] = r;
}

void Document::setElementOption(std::string element, std::string & option, std::string & content) {
	elements[element]->setOption(option, content);
}

void Document::setRendererOption(std::string element, std::string format, std::string option, std::string content) {
	elements[element]->renderers[format]->setOption(option, content);
}

void Document::push(Element * elem) {
	elementStack.push_back(elem);
}

Element * Document::current() {
	return elementStack.back();
}

Element * Document::pop() {
	return elementStack.pop_back();
}

void Document::enter() {
	frameStack.push_back(Frame());
}

void Document::leave() {
	frameStack.pop_back();
}

void Document::setCommand(Command * cmd, std::string name) {
	if (frameStack.back().commands.count(name) == 1) {
		frameSstack.back().commands[name] = cmd
	} else {
		frameStack.back().commands[name] = cmd;
	}
}

Command * getCommand(std::string name) {
	for(std::list<Token>::reverse_iterator rit = frameStack.rbegin(); rit != frameStack.rend(); ++rit) {
		if (rit->commands.count(name) > 0) {
			return rit->commands[name];
		}
	}
	throw Error("Command not found: %s!", name);
}

void Document::setVariable(std::string name, std::string content) {

}

std::string Document::getVariable(std::string name) {

}

void Document::seekElement(Element elem) {
	while (elementStack.back() != elem) {
		if (!elementStack.back()->canPop) {
			throw Error("Cannot pop elemenent %s", elementStack.back()->name);
		} else {
			elementStack.pop_back();
		}
	}
	if (elementStack.back() != elem) {
		throw Error("Element not reached!");
	}
}

void Document::seekElementType(std::string type) {
	while (elementStack.back()->name != type) {
		if (!elementStack.back()->canPop) {
			throw Error("Cannot pop elemene t %s", elementStack.back()->name);
		} else {
			elementStack.pop_back();
		}
	}
	if (elementStack.back()->name != type) {
		throw Error("Element type not reached: %s", type);
	}
}

void Document::seekParagraph() {
	seekElementType("paragraph");
}

void Document::seekParagraphContainer() {
	while (!elementStack.back()->paragraphContainer) {
		if (elementStack.back()->canPop) {
			elementStack.pop_back();
		} else {
			throw Error("Cannot pop elemenet %s", elementStack.back()->name);
		}
	}
	if (!elementStack.back()->paragraphContainer) {
		throw Error("aawqfn")
	}
}

void Document::seekText() {

}

void Document::seekTextContainer() {
	while (!elementStack.back()->textContainer) {
		if (elementStack.back()->canPop) {
			elementStack.pop_back();
		} else {
			throw Error("Cannot pop elemenet %s", elementStack.back()->name);
		}
	}
	if (!elementStack.back()->textContainer) {
		throw Error("aawqfn")
	}
}

void Document::newParagraph() {

}

void Document::execute(std::list<Token> * tokens) {

}
