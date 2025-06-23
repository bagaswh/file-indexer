#include "indexer.h"

int main(int argc, char **argv) {
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <input_filename <index_output_filename>\n", argv[0]);
		return 1;
	}

	FILE *infile = stdin;
	if (strlen(argv[1] > 0) && argv[1][0] != '-') {
		infile = fopen(argv[1], "r");
		if (!infile) {
			perror("fopen");
			return 1;
		}
	}

	FILE *outfile = stdout;
	if (strlen(argv[2] > 0) && argv[2][0] != '-') {
		outfile = fopen(argv[2], "w");
		if (!outfile) {
			perror("fopen");
			return 1;
		}
	}

	Indexer_Index_Builder_Options *options = indexer_index_builder_options_create(DEFAULT_KEY_LEN, NULL, DEFAULT_KEY_FN);

	Indexer_Stream *instream = indexer_stream_create(infile);
	indexer_build_index_stream(instream, outfile);
}