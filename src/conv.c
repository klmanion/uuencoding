/**
 *conv.c
 * source for converting between ascii and uuencoding
 */

#include "conv.h"

#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <math.h>
#include <err.h>
#include <assert.h>
#include "enclist.h"

/* alignment table {{{1
 * 	When characters of 6-bit width are sequenced,
 * 	every fourth character begins at an 8-bit boundary.
 *
 * 		Let n denote the value bits of uuencoded character n,
 *
 * 		00000011 11112222 22333333 44444455 ...
 * 		^                          ^
 *
 * 	There are then three cases for the alignment of 6-bit characters within
 * 	8-bit boundaries.  Each of the 8-bit sequences contains two 6-bit
 * 	sequences either in whole or in part; let them be called let these bits
 * 	be denoted `n' or `m'.
 * 	We can then represent the three alignment cases as follows:
 * 		1: nnnnnnmm
 * 		2: nnnnmmmm
 * 		3: nnmmmmmm
 *
 * 	Each case, then, may be understood by:
 *	o the offset at which the cn's bits must be placed; and
 * 	o masks for obtaining the value bits of n and m.
 */
typedef
struct _alignment
{
	uint8_t	nshift;
	uint8_t	nmask;
	uint8_t	mmask;
} align_t;

static const
align_t tab[] =
{
	{ 2,	0xFC,	0x3 },	/* nnnnnnmm */
	{ 4,	0xF0,	0xF },	/* nnnnmmmm */
	{ 6,	0xC0,	0x3F },	/* nnmmmmmm */
	{ 0 }
};

/* ascii_to_uuenc() {{{1
 * 	For every 6-bit width in an arrary of (8-bit) chars,
 * 	extract these six bits and add 32 (forcing its printability).
 * 	Thus for every 3 chars there will be 4 uuencoded chars.
 */
static
void*
ascii_to_uuenc(
    void *arg)
{
	char *line,*enc;
	char *ch;
	size_t line_len,enc_len;

	assert (arg);

	line = (char *)arg;
	if ((ch=strchr(line, '\n')))
	    *ch = '\0';

	line_len = strlen(line);
	enc_len = ceil(line_len * ((double)4/3));	/* 45..60 chars */
	enc_len += 3;	/* the length, newline, and null */

	enc = (char *)malloc(enc_len * CHAR_BIT);
	if (!enc)
	    errx(1,"malloc failure, %s:%d", __FILE__, __LINE__);

	enc[0] = (char)(line_len + 32);

	for (size_t i=1,j=0; i<enc_len-2; )
	    {
		uint8_t save;

		save = 0;
		for (const align_t *t=tab; t->nshift && j<line_len; ++t,++i,++j)
		    {
			uint8_t n,m;

			n = line[j] & t->nmask;
			m = line[j] & t->mmask;

			n >>= t->nshift;
			save <<= (8 - t->nshift);

			enc[i] = (char)( save | n );
			enc[i] += 32;	/* force printability */

			save = m;

			if (!(t+1)->nshift)
			    enc[++i] = m + 32;
		    }
	    }

	strcpy(strchr(enc, '\0'), "\n");

	return (void *)enc;
}

/* uuenc_to_ascii() {{{1
 * 	Convert a line from uuencoding to ascii encoding.
 * 	Unpack the ascii line length from the front, and
 * 	decompress the 6-bit wide characters into 8-bit widths.
 * 	The line passed as argument terminates with a newline.
 */
static
void*
uuenc_to_ascii(
    void *arg)
{
	char *line,*enc;
	char *ch;
	size_t line_len,enc_len;

	assert (arg);

	line = (char *)arg;
	if ((ch=strrchr(line, '\n')))
	   *ch = '\0'; 

	if (strncmp(line, "begin", 5)==0 /* replace with regex */
		|| strcmp(line, "`")==0
		|| strcmp(line, "end")==0)
	    return NULL;

	line_len = strlen(line);
	enc_len = (size_t)line[0]-32;

	enc = (char *)malloc(enc_len * CHAR_BIT);
	if (!enc)
	    errx(1,"malloc failure, %s:%d", __FILE__, __LINE__);

	for (size_t i=0,j=1; i<enc_len; )
	    {
		for (const align_t *t=tab; t->nshift && j<line_len; ++t,++i,++j)
		    {
			uint8_t n,m;

			n = ((uint8_t)line[j] - 32) & 0x3F;
			n <<= t->nshift;

			if (j+1 < line_len)
			    {
				m = ((uint8_t)line[j+1] - 32) & 0x3F;
				m >>= (6 - t->nshift);
			    }
			else
			    {
				m = 0;
			    }

			enc[i] = (char)( n | m );
		    }
	    }

	return (void *)enc;
}

/* conv() {{{1
 * 	start a thread for each line
 * 	storing the thread ids in order
 * 	in a linked list
 * 	then join them, writting their return value to f_write
 */
void
conv(
    const char *const restrict f_read_name,
    const char *const restrict f_write_name)
{
	FILE * f_read, * f_write;
	char *line;
	enclist_t *el;
	void* (*conv_func)(void *);
	extern uint8_t e_flg;

	f_read = f_read_name? fopen(f_read_name, "r") : stdin;
	f_write = f_write_name? fopen(f_write_name, "w") : stdout;

	if (!f_read)
	    errx(EBADIN, "could not open input file");
	if (!f_write)
	    errx(EBADOUT, "could not open output file");

	(void)enclist_init(&el);

	conv_func = e_flg? ascii_to_uuenc : uuenc_to_ascii;

	if (e_flg)
	    fprintf(f_write, "begin %s %s\n", DEFAULT_PERM,
		    f_read_name? f_read_name : "stdin");

	while (!feof(f_read))
	    {
		line = (char *)malloc(61 * CHAR_BIT);
		if (!line)
		    errx(1,"malloc failure, %s:%d", __FILE__, __LINE__);

		if (fgets(line, 61, f_read))
		    {
			pthread_create(enclist_newthread(el),
				       (pthread_attr_t *)NULL,
				       conv_func,
				       (void *)line);
		    }
		else
		    {
			free(line); line = NULL;
		    }
	    }

	enclist_write(el, f_write);
	el = enclist_free(el);

	if (e_flg)
	    fprintf(f_write, "`\nend\n");
	else
	    fprintf(f_write, "\n");

	fclose(f_write);
	fclose(f_read);

	return;
}
/* }}}1 */

/* vi: set ts=8 sw=8 noexpandtab tw=79: */
