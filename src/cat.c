//cat.c
//

#include <stdio.h>

#include "cat.h"

int
cat(
	const char *file)
{
	char cmd[80];
	snprintf(cmd, 79, "cat %s", file);
	return system(cmd);
}

/* vim: set ts=4 sw=4 noexpandtab tw=79: */
