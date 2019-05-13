#include "gfs.hpp"

Element::Element(std::string n, int l, int c, bool cp, bool tc, bool pc) {
	name = n;
	line = l;
	column = c;
	canPop = cp;
	textContainer = tc;
	paragraphContainer = pc;
}

Element::~Element() {

}

void Element::addChild(Element * child) {
	children.push_back(child);
}

bool Element::hasChildren() {
	return !children.empty();
}

void Element::removeChild(int n) {

}

int Element::countChildren() {
	return children.size();
}

Element * Element::lastChild() {
	return children.back();
}

Element * Element::getChild(int n) {

}

std::list<Element*> Element::getChildren() {
	return children;
}

void Element::dump(std::ostream & out) {

}

void Element::dump(std::ostream & out, int indent) {

}
