#ifndef INDEXER_H
#define INDEXER_H

#include <stdint.h>
#include <stdio.h>

#define INDEX_HEADER_MAGIC_NUMBER 0xB8C97B49
typedef uint64_t OFFSET;
typedef uint64_t LENGTH;

typedef struct Indexer_Index_s Indexer_Index;
typedef struct Indexer_Header_s Indexer_Header;
typedef struct Indexer_Entry_s Indexer_Entry;

typedef struct Indexer_Stream_s Indexer_Stream;

#define DEFAULT_BUFF_SIZE 1 << 17
typedef struct Indexer_Index_Buffer_s Indexer_Index_Buffer;

#define DEFAULT_KEY_LEN 8  // 64 bit
#define DEFAULT_KEY_FN xxhash64_key_fn
typedef struct Indexer_Index_Builder_Options {
	LENGTH key_length;
	Indexer_Index_Buffer *buf;
	uint8_t *(*key_fn)(Indexer_Index_Buffer *buf, OFFSET offset, LENGTH length);
} Indexer_Index_Builder_Options;

// ----- public functions

Indexer_Stream *indexer_stream_create(FILE *file);

// Read index from existing index stream, such as index file.
Indexer_Index *indexer_read_index_from_stream(Indexer_Stream *in);

// Build index from a stream, such as a file, into memory.
Indexer_Index *indexer_build_index_in_mem(Indexer_Stream *in);

// Build index from a stream, such as a file, into another stream.
void indexer_build_index_stream(Indexer_Stream *in, Indexer_Stream *out, Indexer_Index_Builder_Options *options);

Indexer_Index_Buffer *indexer_index_buffer_create(LENGTH size);

// Build options struct.
Indexer_Index_Builder_Options *indexer_index_builder_options_create(LENGTH key_length,
                                                                    Indexer_Index_Buffer *buf,
                                                                    uint8_t *(*key_fn)(Indexer_Index_Buffer *buf, OFFSET offset, LENGTH length));

// Default xxhash keyers

uint8_t *xxhash3_key_fn(Indexer_Index_Buffer *buf, OFFSET offset, LENGTH length);
uint8_t *xxhash32_key_fn(Indexer_Index_Buffer *buf, OFFSET offset, LENGTH length);
uint8_t *xxhash64_key_fn(Indexer_Index_Buffer *buf, OFFSET offset, LENGTH length);
uint8_t *xxhash128_key_fn(Indexer_Index_Buffer *buf, OFFSET offset, LENGTH length);

#endif  // INDEXER_H