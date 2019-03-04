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
typedef struct _enclist enclist_t;

enclist_t*	enclist_init (enclist_t **);
enclist_t*	enclist_free (enclist_t *);

pthread_t*	enclist_newthread (enclist_t *const);
void		enclist_write (const enclist_t *const,FILE *);

#endif /* !_ENCLIST_H_ */

/* vi: set ts=8 sw=8 noexpandtab tw=79: */
