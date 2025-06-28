
#include <stdint.h>

#ifdef __GNUC__
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x) !!(x)
#define unlikely(x) !!(x)
#endif

#define noinline __attribute__((noinline))
#define inline __attribute__((always_inline))

typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int64_t i64;