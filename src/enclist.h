//enclist.h
//

#ifndef __ENCLIST_H__
#define __ENCLIST_H__

#include <stdlib.h>
#include <pthread.h>

/*
 * linked-list structure to dynamically hold encoded lines
 */
typedef
struct _enclist {
	pthread_t thread;
	struct _enclist *cdr;
} enclist_t;
#define enclist_sz (sizeof(enclist_t))

__BEGIN_DECLS
pthread_t*	newthread __P((enclist_t *));
void		writeenc __P((enclist_t *,FILE *));
void*		freeenc __P((enclist_t *));
__END_DECLS

#endif /* !__ENCLIST_H__ */

/* vim: set ts=4 sw=4 noexpandtab tw=79: */
