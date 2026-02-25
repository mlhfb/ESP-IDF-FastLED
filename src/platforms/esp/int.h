#pragma once

// IWYU pragma: private

// For ESP32 IDF < 4.0, we need to include system headers early to get __int32_t/__uint32_t
// This must happen before we define fl:: types to avoid typedef conflicts
#include "platforms/esp/is_esp.h"

#if defined(FL_IS_ESP32)
  #include "platforms/esp/esp_version.h"
  // Pull in toolchain typedef internals (__int32_t, __uint32_t, etc.) so
  // FastLED aliases can match newlib typedefs exactly.
  // IWYU pragma: begin_keep
  #include <sys/types.h>
  // IWYU pragma: end_keep
#endif

///////////////////////////////////////////////////////////////////////////////
// ESP Platform Integer Type Definitions (Xtensa and RISC-V toolchains)
//
// This file defines types for both C and C++. The definitions are logically
// identical; only the namespace wrapper differs between the two languages.
//
// For C++: Types are defined in namespace fl { ... }
// For C: Types are defined in global scope
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// COMMON 16-BIT AND 64-BIT TYPE DEFINITIONS (no platform variation)
// ============================================================================
// These are identical across all ESP32 variants
#define DEFINE_ESP_INT16_64_TYPES \
    typedef short i16; \
    typedef unsigned short u16; \
    typedef long long i64; \
    typedef unsigned long long u64;

// ============================================================================
// 32-BIT TYPE SELECTION (with IDF version handling)
// ============================================================================
// On ESP32 IDF < 4.0, system defines: typedef __int32_t int32_t
// We must use __int32_t for fl::i32 so our typedef is identical
#define DEFINE_ESP_INT32_TYPES \
    PLATFORM_INT32_CONDITIONAL_CHOOSE

// Helper macro for 32-bit type selection based on IDF version
#if defined(FL_IS_ESP32)
  // Prefer toolchain/newlib canonical typedef bases when available.
  #if defined(__int32_t) && defined(__uint32_t)
    #define PLATFORM_INT32_CONDITIONAL_CHOOSE \
        typedef __int32_t i32; \
        typedef __uint32_t u32;
  #elif defined(__INT32_TYPE__) && defined(__UINT32_TYPE__)
    #define PLATFORM_INT32_CONDITIONAL_CHOOSE \
        typedef __INT32_TYPE__ i32; \
        typedef __UINT32_TYPE__ u32;
  #else
    #define PLATFORM_INT32_CONDITIONAL_CHOOSE \
        typedef int i32; \
        typedef unsigned int u32;
  #endif
#elif defined(__INT32_TYPE__) && defined(__UINT32_TYPE__)
    #define PLATFORM_INT32_CONDITIONAL_CHOOSE \
        typedef __INT32_TYPE__ i32; \
        typedef __UINT32_TYPE__ u32;
#else
    // Fallback if compiler doesn't define these (shouldn't happen on modern GCC)
    #define PLATFORM_INT32_CONDITIONAL_CHOOSE \
        typedef int i32; \
        typedef unsigned int u32;
#endif

// ============================================================================
// PLATFORM-SPECIFIC POINTER AND SIZE TYPE DEFINITIONS
// ============================================================================
// These vary by ESP32 variant but use the same pattern:
// All ESP32 variants are 32-bit with unsigned int for size/uptr
#if defined(__SIZE_TYPE__) && defined(__UINTPTR_TYPE__) && defined(__INTPTR_TYPE__) && defined(__PTRDIFF_TYPE__)
  #define DEFINE_ESP_POINTER_TYPES \
      typedef __SIZE_TYPE__ size; \
      typedef __UINTPTR_TYPE__ uptr; \
      typedef __INTPTR_TYPE__ iptr; \
      typedef __PTRDIFF_TYPE__ ptrdiff;
#else
  #define DEFINE_ESP_POINTER_TYPES \
      typedef unsigned int size; \
      typedef unsigned int uptr; \
      typedef int iptr; \
      typedef int ptrdiff;
#endif

///////////////////////////////////////////////////////////////////////////////
// C++ LANGUAGE SUPPORT
///////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
namespace fl {
    // ESP platforms: Xtensa and RISC-V toolchains
    // Each variant has separate definitions to allow independent tuning

    // Common 16-bit and 64-bit types (same across all ESP32 variants)
    DEFINE_ESP_INT16_64_TYPES

    // 32-bit types: Match system stdint.h types exactly to prevent typedef conflicts
    DEFINE_ESP_INT32_TYPES

    // Pointer and size types (identical across all ESP32 variants)
    DEFINE_ESP_POINTER_TYPES
}

///////////////////////////////////////////////////////////////////////////////
// C LANGUAGE SUPPORT
///////////////////////////////////////////////////////////////////////////////
#else
// C language compatibility - define types in global namespace
// Same type definitions as C++ namespace, but without namespace wrapper

// Common types across all ESP32 variants
DEFINE_ESP_INT16_64_TYPES

// 32-bit types: Match system stdint.h types exactly (same logic as C++ section)
DEFINE_ESP_INT32_TYPES

// Platform-specific pointer and size types
DEFINE_ESP_POINTER_TYPES

#endif  // __cplusplus
