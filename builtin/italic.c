#include <gfs/gfs.h>

//Function to create an italic node.
error * italic_function(command * cmd, document * doc, token_list * tokens) {
	return NULL;
};

command italic_command = {
		.execute = &italic_function,
		.private = NULL,
		.free = NULL,
};

error * italic_render_html(runtime * rt, element * elem, FILE * file) {
	fprintf(file, "<i>");
	error * e = render_element_children(rt, elem, "html", file);
	if (e != NULL) {
		return e;
	}
	fprintf(file, "</i>");
	return NULL;
}

error * italic_render_latex(runtime *rt, element * elem, FILE * file) {
	fprintf(file, "\\textit{");
	error * e = render_element_children(rt, elem, "latex", file);
	if (e != NULL) {
		return e;
	}
	fprintf(file, "}");
	return NULL;
}

error * italic_render_xml(runtime * rt, element * elem, FILE * file) {
	fprintf(file, "<ITALIC>");
	error * e = render_element_children(rt, elem, "xml", file);
	if (e != NULL) {
		return e;
	}
	fprintf(file, "</ITALIC>");
	return NULL;
}
void* ELEMENTS[] = {
	(void*)"italic",
	NULL
};

void* DESTRUCTORS[] = {
	(void*)"italic",
	NULL
};

void* COMMANDS[] = {
	(void*)"italic", &italic_command,
	NULL
};

void* RENDERERS[] = {
	(void*)"italic", (void*)"html", &italic_render_html,
	(void*)"italic", (void*)"latex", &italic_render_latex,
	(void*)"italic", (void*)"xml", &italic_render_xml,
	NULL
};
