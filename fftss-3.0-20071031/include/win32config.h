
#if defined(_M_IX86)
#define HAVE_X86_CPUID 1

#define USE_SSE2 1
#define HAVE_EMMINTRIN_H 1

#if defined(__INTEL_COMPILER)

#if __INTEL_COMPILER >= 800
#define HAVE_PMMINTRIN_H 1
#define USE_SSE3 1
#endif

#if __INTEL_COMPILER >= 900
#define HAVE_IA32INTRIN_H 1
#endif

#else

#if defined(_MSC_VER) && (_MSC_VER >= 1400)  /*  Visual Studio 2005 */
#ifndef USE_SSE3
#define USE_SSE3 1
#endif
#define HAVE_INTRIN_H 1
#endif
#endif

#endif  /*  _M_IX86  */

#if defined(_M_X64)

#ifndef __x86_64__
#define __x86_64__ 1
#endif

#define HAVE_X86_CPUID 1

#if !defined(__INTEL_COMPILER)
#define NO_ASM 1
#endif

#define USE_SSE2 1
#define HAVE_EMMINTRIN_H 1

#if defined(__INTEL_COMPILER)

#if __INTEL_COMPILER >= 800
#define USE_SSE3 1
#define HAVE_PMMINTRIN_H 1
#endif

#if __INTEL_COMPILER >= 900
#define HAVE_IA32INTRIN_H 1
#endif

#else

#if defined(_MSC_VER) && (_MSC_VER >= 1400)  /*  Visual Studio 2005 */
#ifndef USE_SSE3
#define USE_SSE3 1
#endif
#define HAVE_INTRIN_H 1
#endif

#endif
#endif  /*  _M_X64  */

#if defined(_M_IA64)

#ifndef __ia64__
#define __ia64__ 1
#endif

#if defined(__INTEL_COMPILER)
#define HAVE_IA64INTRIN_H 1
#else
#define NO_ASM 1
#endif

#endif  /*  _M_IA64  */

/* Define to 1 if you have the <complex.h> header file. */
/* #undef HAVE_COMPLEX_H */

/* Define to 1 if you have the <dlfcn.h> header file. */
/* #define HAVE_DLFCN_H 1 */

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 1

/* internal __creal() */
/* #define HAVE_INTERNAL_CREAL 1 */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `m' library (-lm). */
#define HAVE_LIBM 1

/* Define to 1 if you have the <malloc.h> header file. */
#define HAVE_MALLOC_H 1

/* Define to 1 if you have the <memory.h> header file. */
/* #define HAVE_MEMORY_H 1 */

#define HAVE_ROUND 1

/* Name of package */
#define PACKAGE "fftss"

/* Use wrap440.h for emulate 440d */
/* #define USE_WRAP440D 1 */

/* Version number of package */
#define VERSION "1.0"

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Define to equivalent of C99 restrict keyword, or to nothing if this is not
   supported. Do not define if restrict is supported directly. */
#if defined(__INTEL_COMPILER)
#define restrict __restrict
#else
#define inline
#define restrict
#endif
