//main.c
//created by: Kurt L. Manion
//on: 16 Aug 2016
//problem from: «https://www.reddit.com/r/dailyprogrammer/comments/4xy6i1/20160816_challenge_279_easy_uuencoding/»
//
//uuencoding/uudecoding

//example encoded file that reads "cat"
/*
  begin 644 cat.txt
  #0V%T
  `
  end
*/

#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <string.h>
#include <err.h>
#include <getopt.h>
#include <stdint.h>
#include <sys/types.h>
#include <ctype.h>

#include "encode.h"
#include "decode.h"
#include "cat.h"

//#define _use_kprintf_
#ifdef _use_kprintf_
#define kprintf(...) do{ (void)fprintf(stderr, __VA_ARGS__); }while(0)
#else
#define kprintf(...) /* NULL */
#endif /* _use_kprintf_ */

#define LINE_BUF ((size_t)4096)

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
usage(const char *const basename)
{
	fprintf(stderr, "usage:\n"
		"%s -e -f file_to_encode -o output_file\n"
		"%s -d -f file_to_decode\n", basename, basename);
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
	char *output_file = NULL;
	char *input_file = NULL;
	uint8_t r;
	const char *const basename = argv[0];

	opterr = 0;

	e_flg = d_flg = c_flg = 0;

	while ((flg = getopt_long(argc,argv, opts, longopts, NULL)) != -1) {
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
			usage(basename);;

		case '?':
			if (isprint(optopt))
				warnx("unknown option flag `%c'", (char)optopt);
		 	else
				warnx("unknown option code `%#x'", optopt);
			usage(basename);;

		case ':':
			warnx("option flag `%c' requires an argument", (char)optopt);
			usage(basename);;

		default:
			abort();;
		}
	}
	argc -= optind;
	argv += optind;

	if (!(e_flg ^ d_flg)) {
		warnx("%s", "you must specify the e or d mode");
		usage(basename);
	}
	if (e_flg) //-e -f file -o file
	{
		if (!output_file) { //then stdout will recieve the output
			c_flg = 0;
		}
		if (!input_file && feof(stdin)) {
			warnx("%s", "no input has been provided");
			usage(basename);
		}
		r = encode(input_file, output_file);
		if (c_flg)
			cat(output_file);
		return r;
	}		
	else if (d_flg) //-d -f file
	{
		if (!input_file) {
			warnx("%s", "no input file has been specified");
			usage(basename);
		}
		r = decode(input_file);
		return r;
	}
	return EXIT_FAILURE;
}

/* vim: set ts=4 sw=4 noexpandtab tw=79: */
