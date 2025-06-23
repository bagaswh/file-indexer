#include "indexer.h"

#include <stdio.h>

#define packed __attribute__((packed))

typedef struct Indexer_Index_s {
	Indexer_Header_s header;
	Indexer_Entry_s *entries;
} Indexer_Index_s;

typedef struct packed Indexer_Header_s {
	uint32_t magic_number;
	uint64_t entry_size;
	uint64_t entry_nums;
	uint8_t descriptor;
} Indexer_Header_s;

typedef struct packed Indexer_Entry_s {
	OFFSET offset;
	LENGTH length;
	uint8_t key[];
} Indexer_Entry_s;

/*
 * This does not really do anyhthing dawg. What values do it bring?
 */
typedef struct Indexer_Stream_s {
	FILE *file;
} Indexer_Stream_s;

Indexer_Stream_s *indexer_stream_create(FILE *file) {
	Indexer_Stream_s *stream = malloc(sizeof(Indexer_Stream_s));
	stream->file = file;
	return stream;
}

typedef struct Indexer_Index_Buffer_s {
	uint64_t size;
	void *data;
} Indexer_Index_Buffer_s;

Indexer_Index_Builder_Options *indexer_index_builder_options_create(LENGTH key_length,
                                                                    Indexer_Index_Buffer *buf,
                                                                    uint8_t *(*key_fn)(Indexer_Index_Buffer *buf, OFFSET offset, LENGTH length)) {
	Indexer_Index_Builder_Options *options = malloc(sizeof(Indexer_Index_Builder_Options));
	if (options == NULL) {
		return NULL;
	}

	if (key_length == 0 || buf == NULL || key_fn == NULL) {
		free(options);
		return NULL;
	}

	options->key_length = key_length;
	options->buf = buf;
	options->key_fn = key_fn;
	return options;
}

Indexer_Index_Buffer_s *indexer_index_buffer_create(LENGTH size) {
	Indexer_Index_Buffer_s *buf = malloc(sizeof(Indexer_Index_Buffer_s));
	if (buf == NULL) {
		return NULL;
	}

	buf->size = size;
	buf->data = malloc(size);
	if (buf->data == NULL) {
		free(buf);
		return NULL;
	}

	return buf;
}