#pragma once

#define ARGS_CPP17V 201703L

#if defined(_MSC_VER)
 #define ARGS_WINDOWS
#endif

#if defined(ARGS_WINDOWS)
	#if defined(ARGS_INTERNAL)
		#define ARGS_API __declspec(dllexport)
	#else
		#define ARGS_API __declspec(dllimport)
	#endif
#else
	#error Support for this platform is not complete yet.
#endif

#ifdef __has_cpp_attribute
#  define AHASCPPATTRIB(x) __has_cpp_attribute(x)
#else
#  define AHASCPPATTRIB(x) 0
#endif



#if __cplusplus >= ARGS_CPP17V || AHASCPPATTRIB(nodiscard)
#define A_NODISCARD [[nodiscard]]
#else
#define A_NODISCARD
#endif