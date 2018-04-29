//encode.c
//

#include <stdio.h>
#include <err.h>
#include <string.h>

#include "encode.h"
#include "bitconv.h"
#include "enclist.h"

void
header(
	FILE * fd, 				//output file written to in encode funct
	const char *perm, 		//octal UNIX permissions, usualy 644
	const char *filename) 	//filename to be written to
{
	fprintf(fd, "begin %s %s\n", perm, filename);
}

void
footer(
	FILE *fd)
{
	fprintf(fd, "`\nend\n");
}

/* power-house function that actualy converts text to binary 
 * this is now the thread function
 * --> dose sprintf require the void ?
 * return value needs to be freed
 */
void*
encode_line(
	void *arg)
{
	char *line = (char *)arg;
	uint8_t *bin;
	//the 45->60 chars and the length newline and null
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
 * itself
 */
uint8_t
encode(
	const char *restrict ifile,
	const char *restrict ofile)
{
	FILE * f_read;
	FILE * f_write;
	char *line = NULL;

	if (ifile == NULL) {
		ifile = "stdin";
		f_read = stdin;
	} else {
		f_read = fopen(ifile, "r");
	}

	if (ofile == NULL)
		f_write = stdout;
	else
		f_write = fopen(ofile, "w");

	if (!f_read)
		errx(EBADIN, "%s\n", "could not open input_file");
	if (!f_write)
		errx(EBADOUT, "%s\n", "could not open output_file");

	header(f_write, DEFAULT_PERM, ifile); 

	enclist_t *el = (enclist_t *)malloc(enclist_sz);
	el->cdr = NULL;
	while (readline(&line, f_read) != EOF)
	{
		pthread_create(newthread(el), NULL, &encode_line, (void *)line);

		free(line);
	}
	writeenc(el, f_write);
	el = freeenc(el);

	//add footer
	footer(f_write);

	fclose(f_read);
	fclose(f_write);
	return 0; //stand-in
}

/* vim: set ts=4 sw=4 noexpandtab tw=79: */
