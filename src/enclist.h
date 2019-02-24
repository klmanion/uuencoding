/**
 *enclist.h
 */

#ifndef _ENCLIST_H_
#define _ENCLIST_H_

#include <stdlib.h>
#include <pthread.h>

/**
 * linked-list structure to dynamically hold encoded lines
 */
typedef
struct _enclist {
	pthread_t		 thread;
	struct _enclist	*cdr;
} enclist_t;
#define enclist_sz (sizeof(enclist_t))

__BEGIN_DECLS
pthread_t*	newthread __P((enclist_t *));
void		writeenc __P((enclist_t *,FILE *));
void*		freeenc __P((enclist_t *));
__END_DECLS

#endif /* !_ENCLIST_H_ */

/* vi: set ts=8 sw=8 noexpandtab tw=79: */
