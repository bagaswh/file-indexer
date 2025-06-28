
#include <string.h>

#include "indexer.h"

int build_index(FILE *infile, FILE *outfile) {
}

int main(int argc, char **argv) {
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <input_filename <index_output_filename>\n", argv[0]);
		return 1;
	}

	FILE *infile = stdin;
	if (strlen(argv[1]) > 0 && argv[1][0] != '-') {
		infile = fopen(argv[1], "r");
		if (!infile) {
			perror("fopen");
			return 1;
		}
	}

	FILE *outfile = stdout;
	if (strlen(argv[2]) > 0 && argv[2][0] != '-') {
		outfile = fopen(argv[2], "w");
		if (!outfile) {
			perror("fopen");
			return 1;
		}
	}

	if (!build_index(infile, outfile)) {
		return 1;
	}
}