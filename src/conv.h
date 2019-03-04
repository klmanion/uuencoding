/**
 *conv.h
 * header for converting between ascii and uuencoding
 */

#ifndef _CONV_H_
#define _CONV_H_

#include <stdlib.h>
#include <stdio.h>

#define __default_perm__ "644"
#define DEFAULT_PERM __default_perm__

#define EBADIN 79
#define EBADOUT 80

void	conv (const char *const restrict,const char *const restrict);

#endif /* !_CONV_H_ */

/* vi: set ts=8 sw=8 noexpandtab tw=79: */
