#include "indexer.h"

#include <stdio.h>
#include <stdlib.h>

#define packed __attribute__((packed))

typedef struct packed Indexer_Header_s {
	u32 magic_number;
	u64 entry_size;
	u64 entry_nums;
	u8 descriptor;
} Indexer_Header_s;

typedef struct packed Indexer_Entry_s {
	u64 offset;
	u64 length;
	u64 checksum;
	u8 key[];
} Indexer_Entry_s;

typedef struct Indexer_Index_s {
	Indexer_Header_s header;
	Indexer_Entry_s *entries;
} Indexer_Index_s;

typedef struct Indexer_Ctx_s {
	Indexer_Index *index;
} Indexer_Ctx_s;

bool with_checksum(u8 descriptor) {
	return (descriptor & DESC_WITH_CHECKSUM) != 0;
}

void indexer_create_header(Indexer_Ctx_s *ctx, u64 key_size, u8 descriptor) {
	Indexer_Header *header = malloc(sizeof(Indexer_Header));
	if (!header) {
		return;
	}
	header->magic_number = INDEX_HEADER_MAGIC_NUMBER;
	header->descriptor = descriptor;
	header->entry_size = sizeof(Indexer_Entry) + key_size;
	if (with_checksum(descriptor)) {
		header->entry_size += sizeof(u64);
	}
	header->entry_nums = 0;
	ctx->index->header = *header;
}

void indexer_create_entry(Indexer_Ctx_s *ctx, Indexer_In_Buffer *in_buf, u8 descriptor, Keyer_Fn keyer_fn) {
	Indexer_Entry *entry = malloc(sizeof(Indexer_Entry));
	if (!entry || keyer_fn == NULL) {
		return;
	}
	u8 *key = keyer_fn(in_buf, 0, in_buf->size);
}