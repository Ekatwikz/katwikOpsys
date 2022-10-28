#ifndef EASY_ATTR_H
#define EASY_ATTR_H

#include "options.h"

// makes common attributes easy to type out ig

#if ATTRIBUTES_ALLOWED
#define NON_ATTR_IF(expr) if (false)
#define MY_WARN_UNUSED __attribute__((warn_unused_result))
#define MY_NON_NULL(...) __attribute__(( nonnull(__VA_ARGS__) ))
#define MY_DEPRECATE(msg) __attribute__ (( deprecated (msg) ))
#define MY_FORMAT(...) __attribute__(( format (__VA_ARGS__) ))
#define MY_INLINE __attribute__((always_inline))
#else // ATTRIBUTUES_ALLOWED
#define NON_ATTR_IF(expr) if (expr) /* for some null checks without getting Wnonnull-compare */
#define MY_WARN_UNUSED
#define MY_NON_NULL(...)
#define MY_DEPRECATE(msg)
#define MY_FORMAT(...)
#define MY_INLINE
#endif // ATTRIBUTUES_ALLOWED

#endif // EASY_ATTR_H