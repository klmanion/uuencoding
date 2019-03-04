/**
 *enclist.c
 */

#include "enclist.h"

#include <err.h>
#include <assert.h>

/* struct _enclist {{{1 */
struct _enclist {
	pthread_t	 thread;
	enclist_t	*cdr;
};
#define enclist_sz sizeof(enclist_t)

/* memory management {{{1 */
 
/* enclist_init() {{{2
 * 	allocate memory for an enclist and initiallize its default state
 */
enclist_t*
enclist_init(
    enclist_t **ptr)
{
	enclist_t *el = NULL;

	el = (enclist_t *)malloc(enclist_sz);
	if (!el)
	    errx(1,"malloc failure, %s:%d", __FILE__, __LINE__);

	if (ptr)
	    *ptr = el;

	el->cdr = (enclist_t *)NULL;

	return el;
}

/* enclist_free() {{{2
 * 	free the resources allocated to an enclist,
 * 	and return NULL
 */
enclist_t*
enclist_free(
    enclist_t *el)
{
	if (el)
	    {
		if (el->cdr)
		    (void)enclist_free(el->cdr);

		free(el);
	    }

	return el = NULL;
}

/* enclist_newthread {{{1
 * 	create a new enclist struct at the tail of the linked list,
 * 	and return the pthread within it
 */
pthread_t*
enclist_newthread(
    enclist_t *const el)
{
	assert (el);

	if (!el->cdr)
	    {
		return &enclist_init(&(*el).cdr)->thread;
	    }
	else
	    {
		return enclist_newthread(el->cdr);
	    }
}

/* enclist_write {{{1
 * 	write out the lines encoded by the enclist threads
 */
void
enclist_write(
    const enclist_t *const el,
    FILE * fd)
{
	void *enc = NULL;

	assert (el);

	pthread_join(el->thread, &enc);

	if (enc)
	    {
		fprintf(fd, "%s", (char *)enc);
		free(enc);
	    }

	if (el->cdr)
	    enclist_write(el->cdr, fd);
}
/* }}}1 */

/* vi: set ts=8 sw=8 noexpandtab tw=79: */
