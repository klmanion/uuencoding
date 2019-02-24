//bitconv.c
//

#include "bitconv.h"

#include <stdio.h>
#include <string.h>

/* calculates how many bits the array resulting from stobin will have
 * makeing sure that it has padding bits to make it a multiple of 6 */
size_t
stobin_size(
	const char *s)
{
	size_t l;
	l = (strlen(s))*8;
	//add padding bits
	if (l%6 != 0)
		l += 6 - (l%6);
	return l;
}

/* converts an array of chars to an array of 1s and 0s
 * skips the ending newline
 * returned value must be freed */
uint8_t*
stobin(
	const char *s)
{
	uint8_t *a;
	size_t a_len;
	static const int mask[] = { 0x1, 0x2, 0x4, 0x8,
							0x10, 0x20, 0x40, 0x80 };
	a_len = stobin_size(s);
	a = (uint8_t *)malloc(a_len * sizeof(uint8_t));
	memset(a, 0, a_len);
	for(size_t i=0,j=0,len=strlen(s); i<len; ++i) {
		//for each character to be encoded
		for(int k=7; k>=0; --k,++j) {
			//for each bit in that character
			a[j] = (s[i] & mask[k]) >> k;
		}
	}
	return a;
}

/* takes first 6 elements from uint8_t array and considering them as bits
 * converts them to their integer equivalent */
 uint8_t
 bitstoc(
 	uint8_t *a)
{
	uint8_t acc=0;
	for(int i=0; i<6; ++i)
	{
		acc += a[i] << ((6-1)-i);
	}
	return acc;
}

/* reads at least 45 characters from the file
 * seeking forward
 * !!! this does not capture the newline
 * returns EOF on EOF
 * returns 0 otherwise */
int __attribute__((unused))
readline(
	char **line,
	FILE *fd)
{
	char *x;
	*line = (char *)malloc(47*sizeof(char));
	(void)fgets(*line, 46, fd);
	//strip newline
	x = strchr(*line, '\n');
	if (x != NULL)
		*x = '\0';
	if (feof(fd)) {
		return EOF;
	} else {
		return 0;
	}
}
