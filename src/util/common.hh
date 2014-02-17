#ifndef COMMON_HH
#define COMMON_HH

#include <cstddef>

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define __LITTLE_ENDIAN__ 1
#elif (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define __BIG_ENDIAN__ 1
#endif

namespace endian {
#ifdef __LITTLE_ENDIAN__

static inline u8  le(u8  v) {
    return v;
}
static inline u16 le(u16 v) {
    return v;
}
static inline u32 le(u32 v) {
    return v;
}
static inline u64 le(u64 v) {
    return v;
}
static inline u8  be(u8  v) {
    return v;
}
static inline u16 be(u16 v) {
    return ((v&0xFF)<<8) | (v>>8);
}
static inline u32 be(u32 v) {
    return __builtin_bswap32(v);
}
static inline u64 be(u64 v) {
    return __builtin_bswap64(v);
}

#elif __BIG_ENDIAN__
#error "Big endian architectures not supported, FIXME"
#else
#error "Unknown system endianess"
#endif

}

#endif /* COMMON_HH */
