#include "indexer.h"

#include <stdio.h>
#include <stdlib.h>

#define packed __attribute__((packed))

#ifdef __GNUC__
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x) !!(x)
#define unlikely(x) !!(x)
#endif

#define noinline __attribute__((noinline))
#define inline __attribute__((always_inline))

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

#define min(a, b) (a < b ? a : b)

static void inline zero_fill(u8 *arr, size_t len) {
	memset(arr, 0, len);
}

/* ------------ BEGIN Btree ------------ */
/* ------------ END Btree ------------ */

/* ------------ BEGIN Radix sort ------------ */
#ifdef RADIX_SORT_DEBUG_ENABLED
#define radix_sort_debug_iter_count(c, incr) \
	{                                        \
		c = c + incr;                        \
	};
#define radix_sort_debug_print_iter(c) printf("radix sort iter count: %ld\n", c)
#else
#define radix_sort_debug_iter_count(c, incr)
#define radix_sort_debug_print_iter(c)
#endif

static void radix_sort(u8 *arr,
                       size_t rows,
                       size_t cols,
                       u8 *bucket,
                       u8 *freqbucket,
                       size_t bucket_len) {
	i64 iters = 0;

	for (int j = cols - 1; j >= 0; j--) {
		zero_fill(bucket, bucket_len);
		zero_fill(freqbucket, bucket_len);
		radix_sort_debug_iter_count(iters, bucket_len * 2);

		for (int i = 0; i < rows; i++) {
			u8 digit = arr[i * cols + j];
			freqbucket[digit]++;
		}
		radix_sort_debug_iter_count(iters, rows);

		memcpy(bucket, freqbucket, bucket_len);
		radix_sort_debug_iter_count(iters, bucket_len);

		for (int i = 1; i < bucket_len; i++) {
			bucket[i] += bucket[i - 1];
		}
		radix_sort_debug_iter_count(iters, bucket_len - 1);

		for (int i = bucket_len - 1; i > 0; --i) {
			bucket[i] = bucket[i - 1];
			if (i == 1) {
				bucket[i - 1] = 0;
			}
		}
		radix_sort_debug_iter_count(iters, bucket_len - 2);

		int row = 0;
		for (int i = 0; i < bucket_len; i++) {
			u8 freq = freqbucket[i];
			while (freq > 0) {
				arr[row * cols + j] = i;
				freq--;
				row++;
			}
		}
		radix_sort_debug_iter_count(iters, bucket_len);
	}

	radix_sort_debug_print_iter(iters);
}

/* ------------ END Radix sort ------------ */

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