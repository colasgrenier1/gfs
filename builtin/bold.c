#include <gfs/gfs.h>

//Function to create an italic node.
error * bold_function(command * cmd, document * doc, token_list * tokens) {
	return NULL;
};

command bold_command = {
		.execute = &bold_function,
		.private = NULL,
		.free = NULL,
};

error * bold_render_html(runtime * rt, element * elem, FILE * file) {
	fprintf(file, "<b>");
	error * e = render_element_children(rt, elem, "html", file);
	if (e != NULL) {
		return e;
	}
	fprintf(file, "</b>");
	return NULL;
}

error * bold_render_latex(runtime *rt, element * elem, FILE * file) {
	fprintf(file, "\\textbf{");
	error * e = render_element_children(rt, elem, "latex", file);
	if (e != NULL) {
		return e;
	}
	fprintf(file, "}");
	return NULL;
}

error * bold_render_xml(runtime * rt, element * elem, FILE * file) {
	fprintf(file, "<BOLD>");
	error * e = render_element_children(rt, elem, "xml", file);
	if (e != NULL) {
		return e;
	}
	fprintf(file, "</BOLD>");
	return NULL;
}

void* ELEMENTS[] = {
	(void*)"bold",
	NULL
};

void* DESTRUCTORS[] = {
	(void*)"bold",
	NULL
};

void* COMMANDS[] = {
	(void*)"bold", &bold_command,
	NULL
};

void* RENDERERS[] = {
	(void*)"bold",(void*)"html",&bold_render_html,
	(void*)"bold",(void*)"latex",&bold_render_latex,
	(void*)"bold",(void*)"xml",&bold_render_xml,
	NULL
};
