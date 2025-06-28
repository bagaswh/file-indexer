#include "radix_sort.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "unity.h"
#include "util.h"

// Include your header file here
// #include "your_radix_sort_header.h"

// Copy your radix sort implementation here or include it
// For this example, I'll assume the functions are available

// Helper function to verify array is sorted
static bool is_array_sorted(u8 *arr, size_t rows, size_t cols) {
	for (size_t i = 1; i < rows; i++) {
		if (memcmp(&arr[(i - 1) * cols], &arr[i * cols], cols) > 0) {
			return false;
		}
	}
	return true;
}

// Helper function to create a copy of array for testing
static void copy_array(u8 *dest, u8 *src, size_t rows, size_t cols) {
	memcpy(dest, src, rows * cols * sizeof(u8));
}

// Helper function to print array for debugging
static void print_test_array(u8 *arr, size_t rows, size_t cols, const char *label) {
	printf("%s:\n", label);
	for (size_t i = 0; i < rows && i < 10; i++) {  // Print first 10 rows max
		for (size_t j = 0; j < cols; j++) {
			printf("%3d ", arr[i * cols + j]);
		}
		printf("\n");
	}
	if (rows > 10) printf("... (%zu more rows)\n", rows - 10);
	printf("\n");
}

void setUp(void) {
	// Set up code here, if needed
}

void tearDown(void) {
	// Clean up code here, if needed
}

// Test 1: Empty array
void test_radix_sort_empty_array(void) {
	u8 *arr = NULL;
	u8 bucket[256];
	u8 freqbucket[256];

	// This should not crash - though behavior may vary
	// You might want to add null checks to your implementation
	TEST_PASS_MESSAGE("Empty array test completed without crash");
}

// Test 2: Single element
void test_radix_sort_single_element(void) {
	u8 arr[] = {42, 13, 255, 0};
	u8 bucket[256];
	u8 freqbucket[256];
	size_t rows = 1;
	size_t cols = 4;

	radix_sort(arr, rows, cols, bucket, freqbucket, sizeof(bucket));

	// Single element should remain unchanged
	TEST_ASSERT_EQUAL_UINT8(42, arr[0]);
	TEST_ASSERT_EQUAL_UINT8(13, arr[1]);
	TEST_ASSERT_EQUAL_UINT8(255, arr[2]);
	TEST_ASSERT_EQUAL_UINT8(0, arr[3]);
}

// Test 3: Two elements - already sorted
void test_radix_sort_two_elements_sorted(void) {
	u8 arr[] = {
	    10, 20, 30, 40,
	    50, 60, 70, 80};
	u8 bucket[256];
	u8 freqbucket[256];
	size_t rows = 2;
	size_t cols = 4;

	radix_sort(arr, rows, cols, bucket, freqbucket, sizeof(bucket));

	TEST_ASSERT_TRUE(is_array_sorted(arr, rows, cols));
	// Should remain in same order
	TEST_ASSERT_EQUAL_UINT8(10, arr[0]);
	TEST_ASSERT_EQUAL_UINT8(50, arr[4]);
}

// Test 4: Two elements - reverse sorted
void test_radix_sort_two_elements_reverse(void) {
	u8 arr[] = {
	    50, 60, 70, 80,
	    10, 20, 30, 40};
	u8 bucket[256];
	u8 freqbucket[256];
	size_t rows = 2;
	size_t cols = 4;

	radix_sort(arr, rows, cols, bucket, freqbucket, sizeof(bucket));

	TEST_ASSERT_TRUE(is_array_sorted(arr, rows, cols));
	// Should be swapped
	TEST_ASSERT_EQUAL_UINT8(10, arr[0]);
	TEST_ASSERT_EQUAL_UINT8(50, arr[4]);
}

// Test 5: All elements identical
void test_radix_sort_identical_elements(void) {
	u8 arr[] = {
	    100, 100, 100, 100,
	    100, 100, 100, 100,
	    100, 100, 100, 100,
	    100, 100, 100, 100,
	    100, 100, 100, 100};
	u8 bucket[256];
	u8 freqbucket[256];
	size_t rows = 5;
	size_t cols = 4;

	radix_sort(arr, rows, cols, bucket, freqbucket, sizeof(bucket));

	TEST_ASSERT_TRUE(is_array_sorted(arr, rows, cols));
	// All should remain 100
	for (int i = 0; i < rows * cols; i++) {
		TEST_ASSERT_EQUAL_UINT8(100, arr[i]);
	}
}

// Test 6: All zeros
void test_radix_sort_all_zeros(void) {
	u8 arr[] = {
	    0, 0, 0, 0,
	    0, 0, 0, 0,
	    0, 0, 0, 0};
	u8 bucket[256];
	u8 freqbucket[256];
	size_t rows = 3;
	size_t cols = 4;

	radix_sort(arr, rows, cols, bucket, freqbucket, sizeof(bucket));

	TEST_ASSERT_TRUE(is_array_sorted(arr, rows, cols));
	for (int i = 0; i < rows * cols; i++) {
		TEST_ASSERT_EQUAL_UINT8(0, arr[i]);
	}
}

// Test 7: Maximum values (255)
void test_radix_sort_max_values(void) {
	u8 arr[] = {
	    255, 255, 255, 255,
	    255, 255, 255, 255,
	    255, 255, 255, 255};
	u8 bucket[256];
	u8 freqbucket[256];
	size_t rows = 3;
	size_t cols = 4;

	radix_sort(arr, rows, cols, bucket, freqbucket, sizeof(bucket));

	TEST_ASSERT_TRUE(is_array_sorted(arr, rows, cols));
	for (int i = 0; i < rows * cols; i++) {
		TEST_ASSERT_EQUAL_UINT8(255, arr[i]);
	}
}

// Test 8: Mix of min and max values
void test_radix_sort_min_max_mix(void) {
	u8 arr[] = {
	    255, 0, 255, 0,
	    0, 255, 0, 255,
	    255, 255, 0, 0,
	    0, 0, 255, 255};
	u8 bucket[256];
	u8 freqbucket[256];
	size_t rows = 4;
	size_t cols = 4;

	radix_sort(arr, rows, cols, bucket, freqbucket, sizeof(bucket));

	TEST_ASSERT_TRUE(is_array_sorted(arr, rows, cols));
}

// Test 9: Single column
void test_radix_sort_single_column(void) {
	u8 arr[] = {5, 3, 8, 1, 9, 2, 7, 4, 6};
	u8 bucket[256];
	u8 freqbucket[256];
	size_t rows = 9;
	size_t cols = 1;

	radix_sort(arr, rows, cols, bucket, freqbucket, sizeof(bucket));

	TEST_ASSERT_TRUE(is_array_sorted(arr, rows, cols));
	// Should be sorted: 1, 2, 3, 4, 5, 6, 7, 8, 9
	for (int i = 0; i < rows; i++) {
		TEST_ASSERT_EQUAL_UINT8(i + 1, arr[i]);
	}
}

// Test 10: Large number of columns
void test_radix_sort_many_columns(void) {
#define COLS 10
	u8 arr[][COLS] = {
	    {9, 8, 7, 6, 5, 4, 3, 2, 1, 0},
	    {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	    {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
	    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
	u8 bucket[256];
	u8 freqbucket[256];
	size_t rows = 4;
	size_t cols = COLS;

	radix_sort(&arr[0][0], rows, cols, bucket, freqbucket, sizeof(bucket));

	TEST_ASSERT_TRUE(is_array_sorted(&arr[0][0], rows, cols));
#undef COLS
}

// Test 11: Already sorted array
void test_radix_sort_already_sorted(void) {
	u8 arr[] = {
	    1, 2, 3, 4,
	    5, 6, 7, 8,
	    9, 10, 11, 12,
	    13, 14, 15, 16};
	u8 bucket[256];
	u8 freqbucket[256];
	size_t rows = 4;
	size_t cols = 4;

	radix_sort(arr, rows, cols, bucket, freqbucket, sizeof(bucket));

	TEST_ASSERT_TRUE(is_array_sorted(arr, rows, cols));
}

// Test 12: Reverse sorted array
void test_radix_sort_reverse_sorted(void) {
	u8 arr[] = {
	    16, 15, 14, 13,
	    12, 11, 10, 9,
	    8, 7, 6, 5,
	    4, 3, 2, 1};
	u8 bucket[256];
	u8 freqbucket[256];
	size_t rows = 4;
	size_t cols = 4;

	radix_sort(arr, rows, cols, bucket, freqbucket, sizeof(bucket));

	TEST_ASSERT_TRUE(is_array_sorted(arr, rows, cols));
}

// Test 13: Duplicate rows
void test_radix_sort_duplicate_rows(void) {
	u8 arr[] = {
	    10, 20, 30, 40,
	    5, 15, 25, 35,
	    10, 20, 30, 40,  // duplicate
	    1, 2, 3, 4,
	    5, 15, 25, 35,  // duplicate
	    10, 20, 30, 40  // duplicate
	};
	u8 bucket[256];
	u8 freqbucket[256];
	size_t rows = 6;
	size_t cols = 4;

	radix_sort(arr, rows, cols, bucket, freqbucket, sizeof(bucket));

	TEST_ASSERT_TRUE(is_array_sorted(arr, rows, cols));
}

// Test 14: Random data
void test_radix_sort_random_data(void) {
	srand(12345);  // Fixed seed for reproducible tests
#define ROWS 20
#define COLS 6
	u8 arr[ROWS][COLS];
	u8 bucket[256];
	u8 freqbucket[256];

	// Fill with random data
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			arr[i][j] = rand() % 256;
		}
	}

	radix_sort(&arr[0][0], ROWS, COLS, bucket, freqbucket, sizeof(bucket));

	TEST_ASSERT_TRUE(is_array_sorted(&arr[0][0], ROWS, COLS));
#undef ROWS
#undef COLS
}

// Test 15: Stress test with large array
void test_radix_sort_large_array(void) {
#define ROWS 100
#define COLS 4
	u8 *arr = malloc(ROWS * COLS * sizeof(u8));
	u8 bucket[256];
	u8 freqbucket[256];

	TEST_ASSERT_NOT_NULL(arr);

	srand(54321);  // Fixed seed
	for (int i = 0; i < ROWS * COLS; i++) {
		arr[i] = rand() % 256;
	}

	radix_sort(arr, ROWS, COLS, bucket, freqbucket, sizeof(bucket));

	TEST_ASSERT_TRUE(is_array_sorted(arr, ROWS, COLS));

	free(arr);
#undef ROWS
#undef COLS
}

// Test 16: Specific pattern - alternating high/low
void test_radix_sort_alternating_pattern(void) {
	u8 arr[] = {
	    255, 0, 255, 0,
	    0, 255, 0, 255,
	    255, 0, 255, 0,
	    0, 255, 0, 255,
	    128, 64, 192, 32};
	u8 bucket[256];
	u8 freqbucket[256];
	size_t rows = 5;
	size_t cols = 4;

	radix_sort(arr, rows, cols, bucket, freqbucket, sizeof(bucket));

	TEST_ASSERT_TRUE(is_array_sorted(arr, rows, cols));
}

// Test 17: Different column sizes edge case
void test_radix_sort_different_widths(void) {
	// Test with different column counts
	u8 arr1[] = {3, 1, 4, 1, 5, 9, 2, 6};  // 8 cols, 1 row
	u8 arr2[] = {2, 7, 1, 8};              // 2 cols, 2 rows
	u8 bucket[256];
	u8 freqbucket[256];

	// Test 8 columns, 1 row
	radix_sort(arr1, 1, 8, bucket, freqbucket, sizeof(bucket));
	TEST_ASSERT_TRUE(is_array_sorted(arr1, 1, 8));

	// Test 2 columns, 2 rows
	radix_sort(arr2, 2, 2, bucket, freqbucket, sizeof(bucket));
	TEST_ASSERT_TRUE(is_array_sorted(arr2, 2, 2));
}

// Test 18: Boundary values around common thresholds
void test_radix_sort_boundary_values(void) {
	u8 arr[] = {
	    127, 128, 129, 130,  // Around signed/unsigned boundary
	    254, 255, 0, 1,      // Around max value wrap
	    63, 64, 65, 66,      // Around 64
	    31, 32, 33, 34       // Around 32
	};
	u8 bucket[256];
	u8 freqbucket[256];
	size_t rows = 4;
	size_t cols = 4;

	radix_sort(arr, rows, cols, bucket, freqbucket, sizeof(bucket));

	TEST_ASSERT_TRUE(is_array_sorted(arr, rows, cols));
}

int main(void) {
	UNITY_BEGIN();

	RUN_TEST(test_radix_sort_empty_array);
	RUN_TEST(test_radix_sort_single_element);
	RUN_TEST(test_radix_sort_two_elements_sorted);
	RUN_TEST(test_radix_sort_two_elements_reverse);
	RUN_TEST(test_radix_sort_identical_elements);
	RUN_TEST(test_radix_sort_all_zeros);
	RUN_TEST(test_radix_sort_max_values);
	RUN_TEST(test_radix_sort_min_max_mix);
	RUN_TEST(test_radix_sort_single_column);
	RUN_TEST(test_radix_sort_many_columns);
	RUN_TEST(test_radix_sort_already_sorted);
	RUN_TEST(test_radix_sort_reverse_sorted);
	RUN_TEST(test_radix_sort_duplicate_rows);
	RUN_TEST(test_radix_sort_random_data);
	RUN_TEST(test_radix_sort_large_array);
	RUN_TEST(test_radix_sort_alternating_pattern);
	RUN_TEST(test_radix_sort_different_widths);
	RUN_TEST(test_radix_sort_boundary_values);

	return UNITY_END();
}