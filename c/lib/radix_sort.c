#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

#define MIN(a, b) (a < b ? a : b)
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

static void inline zero_fill(u8 *arr, size_t len) {
	memset(arr, 0, len);
}

/*
    TODO:
    1. Better cacheability (page, TLB, cache line)
       Eliminate any those issues in the single-threaded impl.
    2. Multithreaded
       Need to watch out for the needs of locking and false sharing.
    3. SIMD
    4. GPU
*/
void radix_sort(u8 *arr,
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
