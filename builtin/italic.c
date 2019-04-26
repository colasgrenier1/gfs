#include <gfs/gfs.h>

//Function to create an italic node. (private must be the ecb pointer here)
error * italic_function(document * doc, token_list * tokens, void * private) {
	return NULL;
};

error * italic_render_html(element * elem, FILE * file) {
	fprintf(file, "<i>");
	error * e = render_element_children(elem, "html", file);
	if (e != NULL) {
		return e;
	}
	fprintf(file, "</i>");
	return NULL;
}

error * italic_render_latex(element * elem, FILE * file) {
	fprintf(file, "\\textit{");
	error * e = render_element_children(elem, "latex", file);
	if (e != NULL) {
		return e;
	}
	fprintf(file, "}");
	return NULL;
}

error * italic_render_xml(element * elem, FILE * file) {
	fprintf(file, "<ITALIC>");
	error * e = render_element_children(elem, "xml", file);
	if (e != NULL) {
		return e;
	}
	fprintf(file, "</ITALIC>");
	return NULL;
}

element_control_block * new_italic_ecb() {
	element_control_block * ecb;
	//We get
	ecb = element_control_block_new("italic");
	return ecb;
}

element * italic_new(document * doc, int line, int col) {

}

error * initialize(runtime * rt) {
	element_control_block * ecb = new_italic_ecb();
	runtime_register_command(rt, "italic", new_command(&italic_command, ecb, NULL));
	runtime_register_element(rt, "italic", ecb);
}
