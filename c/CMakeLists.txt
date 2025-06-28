cmake_minimum_required(VERSION 3.15)
project(MyIndexerProject C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Enable testing
enable_testing()

# Default to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type (Debug or Release)" FORCE)
endif()

# Set compile flags per build type
set(COMMON_CLANG_FLAGS "-Wall -Wextra -pedantic -march=native")

set(CMAKE_C_FLAGS_DEBUG   "${COMMON_CLANG_FLAGS} -O0 -g")
set(CMAKE_C_FLAGS_RELEASE "${COMMON_CLANG_FLAGS} -O2 -flto -DNDEBUG")

# Special flags for tests (remove -Werror to be less strict with test code)
set(CMAKE_C_FLAGS_TEST    "-Wall -Wextra -pedantic -O0 -g -DRADIX_SORT_DEBUG_ENABLED")

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/xxHash
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/Unity/src
)

# Unity test framework library
add_library(unity STATIC
    third_party/Unity/src/unity.c
)

target_include_directories(unity PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/Unity/src
)

# Create main indexer library (without main functions)
add_library(indexer STATIC
    lib/indexer.c
    lib/radix_sort.c
    third_party/xxHash/xxhash.c
)

target_include_directories(indexer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

# Build CLI binary
add_executable(build_index
    clis/build_index.c
)

target_link_libraries(build_index PRIVATE indexer)

# ============================================================================
# TEST CONFIGURATION
# ============================================================================

# Create a separate library for radix sort without main function
add_library(radix_sort_lib STATIC
    lib/radix_sort.c
)

target_include_directories(radix_sort_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

# Set compile definitions to exclude main function when building as library
target_compile_definitions(radix_sort_lib PRIVATE RADIX_SORT_LIB_BUILD)

# Function to add a test executable
function(add_test_executable test_name test_source)
    add_executable(${test_name}
        ${test_source}
    )
    
    target_link_libraries(${test_name} PRIVATE 
        unity
        radix_sort_lib
        indexer
    )
    
    target_include_directories(${test_name} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/Unity/src
    )
    
    # Set test-specific compile flags
    target_compile_options(${test_name} PRIVATE 
        -Wall -Wextra -pedantic -O0 -g -DRADIX_SORT_DEBUG_ENABLED
    )
    
    # Add to CTest
    add_test(NAME ${test_name} COMMAND ${test_name})
    
    # Set test properties
    set_tests_properties(${test_name} PROPERTIES
        TIMEOUT 30
        LABELS "unit_test"
    )
endfunction()

# Add radix sort tests
add_test_executable(test_radix_sort lib/radix_sort_test.c)

# Add other tests as needed
# add_test_executable(test_btree lib/btree_test.c)
# add_test_executable(test_indexer lib/indexer_test.c)

# ============================================================================
# CUSTOM TEST TARGETS
# ============================================================================

# Custom target to run all tests
add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose
    DEPENDS test_radix_sort
    COMMENT "Running all tests"
)

# Custom target to run tests with memory checking (if valgrind is available)
find_program(VALGRIND_EXECUTABLE valgrind)
if(VALGRIND_EXECUTABLE)
    add_custom_target(test_memcheck
        COMMAND ${CMAKE_CTEST_COMMAND} -T memcheck --verbose
        DEPENDS test_radix_sort
        COMMENT "Running tests with memory checking"
    )
endif()

# Custom target to run only radix sort tests
add_custom_target(test_radix
    COMMAND $<TARGET_FILE:test_radix_sort>
    DEPENDS test_radix_sort
    COMMENT "Running radix sort tests"
)

# ============================================================================
# PERFORMANCE/BENCHMARK TESTS
# ============================================================================

# Optional: Add performance test executable
add_executable(benchmark_radix_sort
    lib/radix_sort.c
    # Add a separate benchmark file if you create one
    lib/radix_sort_benchmark.c
)

target_include_directories(benchmark_radix_sort PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

target_compile_options(benchmark_radix_sort PRIVATE
    -Wall -Wextra -O3 -DNDEBUG
)

# Add benchmark as a test (but don't run by default)
add_test(NAME benchmark_radix_sort COMMAND benchmark_radix_sort)
set_tests_properties(benchmark_radix_sort PROPERTIES
    TIMEOUT 60
    LABELS "benchmark"
)

# ============================================================================
# CODE COVERAGE (Debug builds only)
# ============================================================================

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Check if we're using GCC or Clang for coverage
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        option(ENABLE_COVERAGE "Enable code coverage reporting" OFF)
        
        if(ENABLE_COVERAGE)
            target_compile_options(indexer PRIVATE --coverage)
            target_link_options(indexer PRIVATE --coverage)
            
            target_compile_options(test_radix_sort PRIVATE --coverage)
            target_link_options(test_radix_sort PRIVATE --coverage)
            
            # Custom target for generating coverage report
            add_custom_target(coverage
                COMMAND ${CMAKE_CTEST_COMMAND}
                COMMAND gcov ${CMAKE_CURRENT_SOURCE_DIR}/lib/*.c
                COMMAND lcov --capture --directory . --output-file coverage.info
                COMMAND genhtml coverage.info --output-directory coverage_html
                DEPENDS test_radix_sort
                COMMENT "Generating code coverage report"
            )
        endif()
    endif()
endif()

# ============================================================================
# INSTALL TARGETS
# ============================================================================

# Install the main binary
install(TARGETS build_index
    RUNTIME DESTINATION bin
)

# Install the library
install(TARGETS indexer
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

# Install headers
install(FILES lib/indexer.h lib/util.h
    DESTINATION include
)

# ============================================================================
# PRINT BUILD INFORMATION
# ============================================================================

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "Unity tests: Enabled")
if(VALGRIND_EXECUTABLE)
    message(STATUS "Valgrind: Found at ${VALGRIND_EXECUTABLE}")
else()
    message(STATUS "Valgrind: Not found")
endif()