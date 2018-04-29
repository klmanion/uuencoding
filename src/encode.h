//encode.h
//

#ifndef __ENCODE_H__
#define __ENCODE_H__

#include <stdlib.h>

#define __default_perm__ "644"
#define DEFAULT_PERM __default_perm__

#define EBADIN 79
#define EBADOUT 80

__BEGIN_DECLS
uint8_t	encode __P((const char *restrict,const char *restrict));
__END_DECLS

#endif /* !__ENCODE_H__ */

/* vim: set ts=4 sw=4 noexpandtab tw=79: */
