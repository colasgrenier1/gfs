#include <gfs/gfs.h>

//Function to create an italic node.
error * bold_function(document * doc, token_list * tokens, void * private) {
	return NULL;
};

command bold_command = {
		.execute = &bold_function,
		.private = NULL,
		.free = NULL,
};

error * bold_render_html(element * elem, FILE * file) {
	fprintf(file, "<b>");
	error * e = render_element_children(elem, "html", file);
	if (e != NULL) {
		return e;
	}
	fprintf(file, "</b>");
	return NULL;
}

error * bold_render_latex(element * elem, FILE * file) {
	fprintf(file, "\\textbf{");
	error * e = render_element_children(elem, "latex", file);
	if (e != NULL) {
		return e;
	}
	fprintf(file, "}");
	return NULL;
}

error * bold_render_xml(element * elem, FILE * file) {
	fprintf(file, "<BOLD>");
	error * e = render_element_children(elem, "xml", file);
	if (e != NULL) {
		return e;
	}
	fprintf(file, "</BOLD>");
	return NULL;
}

element_control_block * new_bold_ecb() {
	element_control_block * ecb;
	//We get
	ecb = element_control_block_new("bold");
	return ecb;
}

error * initialize(runtime * rt) {
	runtime_register_command(rt, "bold", &bold_command);
	runtime_register_element(rt, "bold", new_bold_ecb());
}
