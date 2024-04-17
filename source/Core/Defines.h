//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#pragma once
#include <cassert>


// Defines
#define _CASE_ENGINE_STRINGIFY_IMPL(a) #a
#define _CASE_ENGINE_CONCAT_IMPL(x, y) x##y

#define CASE_ENGINE_STRINGIFY(a) _CASE_ENGINE_STRINGIFY_IMPL(a)
#define CASE_ENGINE_CONCAT(x, y) _CASE_ENGINE_CONCAT_IMPL( x, y )

#define CASE_ENGINE_ASSERT(expr) assert(expr)
#define CASE_ENGINE_ASSERT_MSG(expr, msg) assert(expr && msg)
#define CASE_ENGINE_OPTIMIZE_ON  #pragma optimize("", on)
#define CASE_ENGINE_OPTIMIZE_OFF #pragma optimize("", off)
#define CASE_ENGINE_WARNINGS_OFF #pragma(warning(push, 0))
#define CASE_ENGINE_WARNINGS_ON  #pragma(warning(pop))
#define CASE_ENGINE_DEBUGBREAK() __debugbreak()
#define CASE_ENGINE_FORCEINLINE __forceinline
#define CASE_ENGINE_UNREACHABLE() __assume(false)

#define CASE_ENGINE_NODISCARD				[[nodiscard]]
#define CASE_ENGINE_NORETURN				[[noreturn]]
#define CASE_ENGINE_DEPRECATED			[[deprecated]]
#define CASE_ENGINE_DEPRECATED_MSG(msg)	[[deprecated(#msg)]]
#define CASE_ENGINE_ALIGN(align)           alignas(align) 