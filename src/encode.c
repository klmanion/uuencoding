/**
 *encode.c
 * source for encoding routine
 */

#include "encode.h"

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <err.h>
#include "bitconv.h"
#include "enclist.h"

/* power-house function that actualy converts text to binary 
 * this is now the thread function
 * --> dose sprintf require the void ?
 * return value needs to be freed */
static	void*
encode_line(
    void *arg)
{
	char *line = (char *)arg;
	uint8_t *bin;
	//the 45..60 chars and the length newline and null
	char *enc = (char *)malloc(63*sizeof(char));

	//encode the number of characters
	sprintf(enc, "%c", (int)(strlen(line)+32));

	//convert the string of chars into an array of bits
	bin = stobin(line);//must be freed
	for(size_t i=0,len=stobin_size(line); i<len; i+=6)
	    {
		sprintf(enc, "%s%c", enc, bitstoc(&bin[i]) + 32);
	    }
	sprintf(enc, "%s\n", enc);
	free(bin);
	return (void *)enc;
}

/* start a thread for each line
 * storing the thread ids in order
 * in a linked list
 * then join them, writting their return value to f_write
 * change the encode_line funct to only return an encoded version
 * of line (having no aftereffects, since it no longer writes to the file
 * itself */
uint8_t
encode(
    const char *const restrict f_read_name,
    const char *const restrict f_write_name)
{
	FILE * f_read;
	FILE * f_write;
	enclist_t *el;
	char *line = NULL;

	f_read = f_read_name? fopen(f_read_name, "r") : stdin;
	f_write = f_write_name? fopen(f_write_name, "w") : stdout;

	if (!f_read)
	    errx(EBADIN, "could not open input_file");
	if (!f_write)
	    errx(EBADOUT, "could not open output_file");

	el = (enclist_t *)malloc(enclist_sz);
	if (!el)
	    errx(1,"malloc failure, %s:%d", __FILE__, __LINE__);

	el->cdr = NULL;

	/* header */
	fprintf(f_write, "begin %s %s\n", DEFAULT_PERM, f_read_name);

	/* body */
	while (!feof(f_read))
	    {
		line = (char *)malloc(47 * CHAR_BIT);
		if (!line)
		    errx(1,"malloc failure, %s:%d", __FILE__, __LINE__);

		if (fgets(line, 47, f_read))
		    {
			pthread_create(newthread(el), NULL,
				       encode_line, (void *)line);
		    }
		else
		    {
			free(line); line = (char *)NULL;
		    }
	    }
	writeenc(el, f_write);
	el = freeenc(el);

	/* footer */
	fprintf(f_write, "`\nend\n");

	fclose(f_read);
	fclose(f_write);

	return 0;
}

/* vi: set ts=8 sw=8 noexpandtab tw=79: */
