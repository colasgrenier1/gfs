#include <string.h>

#include "error.h"
#include "renderer.h"

error * renderer_set_option(renderer * r, char * opt, void * body) {

}

error * renderer_render(renderer * r, runtime * rt, element * elem, FILE * file) {

}

void renderer_free(renderer * r) {
	if (r->free != NULL) {
		r->free(r);
	}
	free(r);
}

/*******************************************************************************
 *                                                                             *
 *                        R E N D E R E R   T A B L E                          *
 *                                                                             *
 ******************************************************************************/

renderer_table * renderer_table_new() {
	renderer_table * t = (renderer_table*)malloc(sizeof(renderer_table));
	t->size = -1;
	t->alloc = RENDERER_TABLE_DEFAULT_SIZE;
	t->formats = (char**)malloc(sizeof(char *)*RENDERER_TABLE_DEFAULT_SIZE);
	t->renderers = (renderer**)malloc(sizeof(renderer*)*RENDERER_TABLE_DEFAULT_SIZE);
}

void * renderer_table_add(renderer_table * rt, char * fmt, renderer * r) {
	//We see if the renderer is inside right now.
	int idx;
	for (idx=0; idx<rt->size; idx++) {
		if (strcmp(rt->formats[idx], fmt)==0) {
			rt->renderers[idx] = r;
		}
	}
	//We have fallen through: need to resize if needed
	if (rt->size >= rt->alloc - 1) {
		char ** oldf = rt->formats;
		renderer ** oldr = rt->renderers;
		rt->formats = malloc(sizeof(char*)*(rt->alloc+RENDERER_TABLE_INCREMENT));
		rt->renderers = malloc(sizeof(renderer*)*(rt->alloc+RENDERER_TABLE_INCREMENT));
		memcpy(rt->formats, oldf, sizeof(char*)*rt->size);
		memcpy(rt->renderers, oldr, sizeof(renderer*)*rt->size);
		free(oldf);
		free(oldr);
	}
	rt->size += 1;
	rt->formats[rt->size] = fmt;
	rt->renderers[rt->size] = r;
}

renderer * renderer_table_get(renderer_table * rt, char * fmt) {
	int i;
	for (i=0; i<rt->size; i++) {
		if (strcmp(rt->formats[i], fmt)==0) {
			return rt->renderers[i];
		}
	}
	return NULL;
}
