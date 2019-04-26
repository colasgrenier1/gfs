#include <stdbool.h>

#include "element.h"
#include "runtime.h"

error * paragraph_render_html(element * elem, FILE * file) {
	fprintf(file, "<p>");
	error * e = render_element_children(elem, "html", file);
	if (e != NULL) {
		return e;
	}
	fprintf(file, "</p>\n");
}

error * paragraph_render_latex(element * elem, FILE * file) {
	error * e = render_element_children(elem, "latex", file);
	if (e != NULL) {
		return e;
	}
	fprintf(file, "\n\n");
}

error * paragraph_render_xml(element * elem, FILE * file) {
	fprintf(file, "<paragraph>");
	error * e = render_element_children(elem, "xml", file);
	if (e != NULL) {
		return e;
	}
	fprintf(file, "</paragraph>\n");

}

element_control_block * paragraph_ecb_new() {
	element_control_block * ecb;
	//We get
	ecb = element_control_block_new("italic");
	return ecb;
}

element * paragraph_new(runtime * rt, int line, int col) {
	element * e = element_new("paragraph", runtime_element_control_block_get(rt, "paragraph"), line, col);
	e->text_container = true;
	return e;
}

/**
 * Initialize the paragraph element onto the runtime.
 */
error * initialize_paragraph(runtime * rt) {
	runtime_register_element(rt, "paragraph", paragraph_ecb_new());
}
