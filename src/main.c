/**
 *main.c
 * created by: Kurt L. Manion
 * on: 16 Aug 2016
 * modified: 22 Feburary 2019
 * uuencoding/uudecoding
 *
 * example encoded file that reads "cat":
 *	begin 644 cat.txt
 *	#0V%T
 *	`
 *	end
 */

#include <stdlib.h>
#include <stdio.h>
#include <sysexits.h>
#include <string.h>
#include <err.h>
#include <getopt.h>
#include <stdint.h>
#include <sys/types.h>
#include <ctype.h>

#include "encode.h"
#include "decode.h"

const char *const opts = ":edo:f:ch";
const struct option longopts[] = {
	{ "encode",	no_argument,		NULL,	'e' },
	{ "decode",	no_argument,		NULL,	'd' },
	{ "output",	required_argument,	NULL,	'o' },
	{ "file",	required_argument,	NULL,	'f' },
	{ "cat",	no_argument,		NULL,	'c' },
	{ "help",	no_argument,		NULL,	'h' },
	{ NULL, 0, NULL, 0 }
};

void __dead2
usage(void)
{
	const char *const basename = getprogname();

	fprintf(stderr, "usage:\n"
		"%s -e -f file_to_encode [-o output_file]\n"
		"%s -d -f file_to_decode [-o output_file]\n",
		basename, basename);
	exit(EX_USAGE);
}

int
main(
    int argc,
    char *const argv[])
{
	extern int optind;
	extern char *optarg;
	extern int optopt;
	extern int opterr;
	char flg;
	uint8_t e_flg, d_flg, c_flg;
	char *input_file = NULL, *output_file = NULL;
	char com[80];

	opterr = 0;

	setprogname(argv[0]);

	e_flg = d_flg = c_flg = 0;

	while ((flg = getopt_long(argc,argv, opts, longopts, NULL)) != -1)
	    {
		switch (flg) {
		case 'e':
			e_flg = 1;
			break;;

		case 'd':
			d_flg = 1;
			break;;

		case 'o':
			output_file = optarg;
			break;;

		case 'f':
			input_file = optarg;
			break;;

		case 'c':
			c_flg = 1;
			break;;

		case 'h':
			usage();;

		case '?':
			if (isprint(optopt))
			    warnx("unknown option flag `%c'", (char)optopt);
			else
			    warnx("unknown option code `%#x'", (unsigned)optopt);
			usage();;

		case ':':
			warnx("option flag `%c' requires an argument", (char)optopt);
			usage();;

		default:
			abort();;
		}
	    }
	argc -= optind;
	argv += optind;

	if (!(e_flg ^ d_flg))
	    warnx("you must specify either the e or d mode"), usage();

	if (!input_file && feof(stdin))
	    warnx("no input has been provided"), usage();

	if (e_flg) /* -e -f file -o file */
	    {
		encode(input_file, output_file);
	    }		
	else if (d_flg) /* -d -f file */
	    {
		decode(input_file, output_file);
	    }

	if (c_flg && output_file)
	    {
		snprintf(com, 79, "cat %s", output_file);
		system(com);
	    }

	return EXIT_SUCCESS;
}

/* vi: set ts=8 sw=8 noexpandtab tw=79: */
