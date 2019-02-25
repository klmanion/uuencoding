/**
 *enclist.h
 */

#ifndef _ENCLIST_H_
#define _ENCLIST_H_

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

/**
 * linked-list structure to dynamically hold encoded lines
 */
typedef
struct _enclist {
	pthread_t	 thread;
	struct _enclist	*cdr;
} enclist_t;
#define enclist_sz sizeof(enclist_t)

pthread_t*	enclist_newthread (enclist_t *);
void		enclist_write (enclist_t *,FILE *);
void*		enclist_free (enclist_t *);

#endif /* !_ENCLIST_H_ */

/* vi: set ts=8 sw=8 noexpandtab tw=79: */
