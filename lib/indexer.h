#ifndef INDEXER_H
#define INDEXER_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define INDEX_HEADER_MAGIC_NUMBER 0xB8C97B49

typedef struct Indexer_Index_s Indexer_Index;
typedef struct Indexer_Header_s Indexer_Header;
typedef struct Indexer_Entry_s Indexer_Entry;

#define DEFAULT_BUFF_SIZE 1 << 17
typedef struct Indexer_In_Buffer_s {
	const void *src; /**< pointer to input buffer */
	size_t size;     /**< size of input buffer */
	size_t pos;      /**< position where reading stopped. Will be updated. Necessarily 0 <= pos <= size */
} Indexer_In_Buffer;

typedef struct Indexer_Out_Buffer_s {
	void *dst;   /**< start of output buffer */
	size_t size; /**< size of output buffer */
	size_t pos;  /**< position where writing stopped. Will be updated. Necessarily 0 <= pos <= size */
} Indexer_Out_Buffer;

typedef struct Indexer_Ctx_s Indexer_Ctx_s;

#define DEFAULT_KEY_LEN 8  // 64 bit
#define DEFAULT_KEY_FN xxhash64_key_fn

typedef uint8_t *(*Keyer_Fn)(Indexer_In_Buffer *buf, u64 offset, u64 length);

#define DESC_WITH_CHECKSUM 0b00000001

void indexer_create_header(Indexer_Ctx_s *ctx, u64 key_size, u8 descriptor);

// Create index entry from the entire data in in_buf buffer.
void indexer_create_entry(Indexer_Ctx_s *ctx, Indexer_In_Buffer *in_buf, u8 descriptor, Keyer_Fn keyer_fn);

// Default xxhash keyers

u8 *xxhash3_key_fn(Indexer_In_Buffer *buf, u64 offset, u64 length);
u8 *xxhash32_key_fn(Indexer_In_Buffer *buf, u64 offset, u64 length);
u8 *xxhash64_key_fn(Indexer_In_Buffer *buf, u64 offset, u64 length);
u8 *xxhash128_key_fn(Indexer_In_Buffer *buf, u64 offset, u64 length);

#endif  // INDEXER_H