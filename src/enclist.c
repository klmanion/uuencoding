//enclist.c
//

#include <stdio.h>
#include <err.h>

#include "enclist.h"

/* debugging purposes only */
int __attribute__((unused))
enclist_length(
	enclist_t *el)
{
	if (el && el->cdr) {
		return 1 + enclist_length(el->cdr);
	} else if (el) {
		return 1;
	} else {
		return 0;
	}
}
		

pthread_t*
newthread(
	enclist_t *el)
{
	if (el && el->cdr != NULL) {
		return newthread(el->cdr);
	} else if (el) {
		(*el).cdr = (enclist_t *)malloc(enclist_sz);
		el->cdr->cdr = NULL;
		return &el->cdr->thread;
	} else {
		warnx("%s\n", "newthread() called with null enclist_t");
		return NULL;
	}
}

void
writeenc(
	enclist_t *el,
	FILE * fd)
{
	void *enc = NULL;
	pthread_join(el->thread, &enc);
	if (enc)
		fprintf(fd, "%s", (char *)enc);
	free(enc);
	if (el->cdr)
		writeenc(el->cdr, fd);
}

void*
freeenc(
	enclist_t *el)
{
	if (el->cdr)
		el->cdr = freeenc(el->cdr);
	free(el);
	return el = NULL;
}

/* vi: set ts=4 sw=4 noexpandtab tw=79: */
