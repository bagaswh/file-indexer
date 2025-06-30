// lib/radix_sort_benchmark.c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int64_t i64;

// Helper function to generate random data
static void generate_random_data(u8 *arr, size_t rows, size_t cols, unsigned int seed) {
	srand(seed);
	for (size_t i = 0; i < rows * cols; i++) {
		arr[i] = rand() % 256;
	}
}

// Helper function to measure time
static double get_time_diff(struct timespec start, struct timespec end) {
	return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

// Benchmark function
static void benchmark_radix_sort(size_t rows, size_t cols, int iterations) {
	printf("Benchmarking radix sort with %zu rows x %zu cols, %d iterations\n",
	       rows, cols, iterations);

	u8 *arr = malloc(rows * cols * sizeof(u8));
	u8 bucket[256];
	u8 freqbucket[256];

	if (!arr) {
		fprintf(stderr, "Memory allocation failed\n");
		return;
	}

	double total_time = 0.0;
	struct timespec start, end;

	for (int i = 0; i < iterations; i++) {
		// Generate fresh random data for each iteration
		generate_random_data(arr, rows, cols, i + 12345);

		clock_gettime(CLOCK_MONOTONIC, &start);
		radix_sort(arr, rows, cols, bucket, freqbucket, sizeof(bucket));
		clock_gettime(CLOCK_MONOTONIC, &end);

		total_time += get_time_diff(start, end);
	}

	double avg_time = total_time / iterations;
	double elements_per_sec = (rows * cols) / avg_time;

	printf("  Average time: %.6f seconds\n", avg_time);
	printf("  Total time: %.6f seconds\n", total_time);
	printf("  Elements/second: %.0f\n", elements_per_sec);
	printf("  Throughput: %.2f MB/s\n", (elements_per_sec / (1024 * 1024)));
	printf("\n");

	free(arr);
}

int main(void) {
	printf("Radix Sort Performance Benchmark\n");
	printf("=================================\n\n");

	// Test different array sizes
	struct test_case {
		size_t rows, cols;
		int iterations;
	};
	struct test_case tcs[] = {
	    {10, 4, 10000},   // Small arrays, many iterations
	    {100, 4, 1000},   // Medium arrays
	    {1000, 4, 100},   // Large arrays
	    {10000, 4, 10},   // Very large arrays
	    {100, 10, 1000},  // Different column count
	    {1000, 8, 100},   // More columns
	    {50000, 4, 5}     // Stress test
	};

	size_t num_tests = sizeof(tcs) / sizeof(tcs[0]);
	for (size_t i = 0; i < num_tests; i++) {
		benchmark_radix_sort(tcs[i].rows, tcs[i].cols, tcs[i].iterations);
	}

	// second test
	struct test_case tcs2[] = {
	    {1000000000, 8, 1}};

	size_t num_tests2 = sizeof(tcs2) / sizeof(tcs2[0]);
	for (size_t i = 0; i < num_tests2; i++) {
		benchmark_radix_sort(tcs2[i].rows, tcs2[i].cols, tcs2[i].iterations);
	}

	return 0;
}