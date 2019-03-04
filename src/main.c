/**
 *main.c
 * created by: Kurt L. Manion
 * on: 16 Aug 2016
 * modified: 22 Feburary 2019
 * uuencoding/uudecoding
 *
 * example encoded file that reads "Cat":
 *	begin 644 cat.txt
 *	#0V%T
 *	`
 *	end
 */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifndef DEBUG
# define NDEBUG
#endif /* DEBUG */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sysexits.h>
#include <getopt.h>
#include <err.h>

#include "conv.h"

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

	uint8_t	e_flg;
	uint8_t	d_flg;
static	uint8_t	c_flg;

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
	char *f_read_name = NULL, *f_write_name = NULL;
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
			f_write_name = optarg;
			break;;

		case 'f':
			f_read_name = optarg;
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

	if (!f_read_name && feof(stdin))
	    warnx("no input has been provided"), usage();

	conv(f_read_name, f_write_name);

	if (c_flg && f_write_name)
	    {
		snprintf(com, 79, "cat %s", f_write_name);
		system(com);
	    }

	return EXIT_SUCCESS;
}

/* vi: set ts=8 sw=8 noexpandtab tw=79: */
