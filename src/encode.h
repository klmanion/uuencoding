//encode.h
//

#ifndef _ENCODE_H_
#define _ENCODE_H_

#include <stdlib.h>

#define __default_perm__ "644"
#define DEFAULT_PERM __default_perm__

#define EBADIN 79
#define EBADOUT 80

uint8_t	encode (const char *const restrict,const char *const restrict);

#endif /* !_ENCODE_H_ */

/* vi: set ts=8 sw=8 noexpandtab tw=79: */
