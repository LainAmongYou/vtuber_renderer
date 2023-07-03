module;

#define CHECK_TYPE(in_type, out_type) std::is_same<(out_type), (in_type)>::value

export module core;

import cstd;

//
// Type aliasing
//

export using s8  = int8_t;
export using s16 = int16_t;
export using s32 = int32_t;
export using s64 = int64_t;

export using u8  = uint8_t;
export using u16 = uint16_t;
export using u32 = uint32_t;
export using u64 = uint64_t;

export using f32 = float;
export using f64 = double;

export using sptr = intptr_t;
export using uptr = uintptr_t;

export union u128
{
    struct { s64 upper, lower; };
    s64 bits[2];
};

// this might have to be inline functions
export constexpr u64 _KB(const u64 bytes) { return     bytes  * 1024; }
export constexpr u64 _MB(const u64 bytes) { return _KB(bytes) * 1024; }
export constexpr u64 _GB(const u64 bytes) { return _MB(bytes) * 1024; }

export constexpr u64 _64KB  = _KB(64);
export constexpr u64 _1MB   = _MB(1);
export constexpr u64 _2MB   = _MB(2);
export constexpr u64 _4MB   = _MB(4);
export constexpr u64 _8MB   = _MB(8);
export constexpr u64 _16MB  = _MB(16);
export constexpr u64 _32MB  = _MB(32);
export constexpr u64 _64MB  = _MB(64);
export constexpr u64 _128MB = _MB(128);
export constexpr u64 _256MB = _MB(256);
export constexpr u64 _1GB   = _GB(1);

//
// Helpers
//

// Test if the system is Big or Little Endian
#define LITTLE_ENDIAN 0x41424344UL 
#define ENDIAN_ORDER  ('ABCD')
export constexpr bool IS_LITTLE_ENDIAN = (ENDIAN_ORDER == LITTLE_ENDIAN);

export template<typename T>
inline T as_bit(const s64 bit_index)
{
    static_assert(
        CHECK_TYPE(T, s8) || CHECK_TYPE(T, s16) || CHECK_TYPE(T, s32) || CHECK_TYPE(T, s64) ||
        CHECK_TYPE(T, u8) || CHECK_TYPE(T, u16) || CHECK_TYPE(T, u32) || CHECK_TYPE(T, u64),
        "Invalid bit shift type"
    );

    static_assert(
        (sizeof(T) * 8) > bit_index,
        "Attempting to left shift past bounds."
    );

    return static_cast<T>(1 << bit_index);
}

export template<typename T>
inline T bit_toggle(const T value, const s64 bit_index, const s8 bit_value)
{
    static_assert(
        CHECK_TYPE(T, s8) || CHECK_TYPE(T, s16) || CHECK_TYPE(T, s32) || CHECK_TYPE(T, s64) ||
        CHECK_TYPE(T, u8) || CHECK_TYPE(T, u16) || CHECK_TYPE(T, u32) || CHECK_TYPE(T, u64),
        "Invalid bit toggle type"
    );

    static_assert(
        (sizeof(T) * 8) > bit_index,
        "Attempting to set bit out of bounds."
    );

    return (value & ~(1 << bit_index)) | (bit_value << bit_index);
}

//
// Round up to the next highest power of 2.
// @source: http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
//
export inline u32 next_highest_pow2(const u32 value)
{
    u32 result = value;

    result--;
    result |= result >> 1;
    result |= result >> 2;
    result |= result >> 4;
    result |= result >> 8;
    result |= result >> 16;
    result++;

    return result;
}

export inline u64 next_highest_pow2(const u64 value)
{
    u64 result = value;

    result--;
    result |= result >> 1;
    result |= result >> 2;
    result |= result >> 4;
    result |= result >> 8;
    result |= result >> 16;
    result |= result >> 32;
    result++;
    
    return result;
}