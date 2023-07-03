module;

export module cstd;

//
// Export any C++ headers here.
//

export import <math.h>;
export import <float.h>;

export import <cstdint>;
export import <variant>;
export import <type_traits>;
export import <iostream>;
export import <memory>;
export import <cstdlib>;
export import <array>;
export import <vector>;
export import <cassert>;
export import <optional>;

export constexpr uint8_t  U8_MAX             =  UINT8_MAX;
export constexpr uint16_t U16_MAX            = UINT16_MAX;
export constexpr uint32_t U32_MAX            = UINT32_MAX;
export constexpr uint64_t U64_MAX            = UINT64_MAX;
export constexpr int32_t  S32_MAX            = INT32_MAX;
export constexpr int64_t  S64_MAX            = INT64_MAX;
export constexpr float    F32_MIN            = -FLT_MAX;
export constexpr float    F32_MAX            = FLT_MAX;
export constexpr double   F64_MIN            = -DBL_MAX;
export constexpr double   F64_MAX            = DBL_MAX;
export constexpr uint64_t OUTPUT_BUFFER_SIZE = 2048;
