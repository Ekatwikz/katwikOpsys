#ifndef EXTRA_ATTR_H
#define EXTRA_ATTR_H

#include "options.h"

// moi extra attribs :)

#if ATTRIBUTES_ALLOWED
#define NON_ATTR_IF(expr) if (false)
#define MY_WARN_UNUSED __attribute__((warn_unused_result))
#define MY_NON_NULL(...) __attribute__(( nonnull(__VA_ARGS__) ))
#define MY_DEPRECATE(msg) __attribute__ (( deprecated (msg) ))
#define MY_FORMAT(...) __attribute__(( format (__VA_ARGS__) ))
#define MY_INLINE __attribute__((always_inline))
#else // ATTRIBUTUES_ALLOWED
#define NON_ATTR_IF(expr) if (expr) /* for some parameter checks without getting Wnonnull-compare */
#define MY_WARN_UNUSED
#define MY_NON_NULL(...)
#define MY_DEPRECATE(msg)
#define MY_FORMAT(...)
#define MY_INLINE
#endif // ATTRIBUTUES_ALLOWED

#endif // EXTRA_ATTR_H