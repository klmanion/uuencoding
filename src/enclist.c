/**
 *enclist.c
 */

#include "enclist.h"

#include <err.h>

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
enclist_newthread(
    enclist_t *const el)
{
	if (!el)
	    {
		return (pthread_t *)NULL;
	    }
	else if (el->cdr)
	    {
		return enclist_newthread(el->cdr);
	    }
	else
	    {
		(*el).cdr = (enclist_t *)malloc(enclist_sz);
		el->cdr->cdr = (enclist_t *)NULL;
		return &el->cdr->thread;
	    }
}

void
enclist_write(
    enclist_t *el,
    FILE * fd)
{
	void *enc = NULL;

	if (el)
	    {
		pthread_join(el->thread, &enc);

		if (enc)
		    {
			fprintf(fd, "%s", (char *)enc);
			free(enc);
		    }

		if (el->cdr)
		    enclist_write(el->cdr, fd);
	    }
}

void*
enclist_free(
    enclist_t *el)
{
	if (el)
	    {
		if (el->cdr)
		    enclist_free(el->cdr);

		free(el);
	    }

	return el = NULL;
}

/* vi: set ts=8 sw=8 noexpandtab tw=79: */
