//bitconv.h
//

#ifndef __BITCONV_H__
#define __BITCONV_H__

#include <stdlib.h>

__BEGIN_DECLS
size_t		stobin_size __P((const char *));
uint8_t*	stobin __P((const char *));
uint8_t		bitstoc __P((uint8_t *));

int	readline __P((char **,FILE *));
__END_DECLS

#endif /* !__BITCONV_H__ */

/* vim: set ts=4 sw=4 noexpandtab tw=79: */ 
