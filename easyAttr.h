#ifndef EASY_ATTR_H
#define EASY_ATTR_H

#include "options.h"

// seems dumb but might want to also have the option to disable these
// even when __GNUC__ is defined 
#ifndef ATTRIBUTES_ALLOWED
#define ATTRIBUTES_ALLOWED 1
#elif ATTRIBUTES_ALLOWED == 1
#if MUNDANE_MESSAGES
#pragma message "Attributes have been manually allowed"
#endif // MUNDANE_MESSAGES
#elif ATTRIBUTES_ALLOWED == 0
#if MUNDANE_MESSAGES
#pragma message "Attributes have been manually disallowed"
#endif // MUNDANE_MESSAGES
#else
#pragma GCC error "ATTRIBUTES_ALLOWED macro bad value.\nValid values are 1 (allowed) or 0 (disallowed)"
#undef HEADER_OKAY
#endif // ATTRIBUTES_ALLOWED

// makes common attributes easy to type out ig, might rework or delete this file tbh

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