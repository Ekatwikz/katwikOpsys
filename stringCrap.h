#ifndef STRING_CRAP_H
#define STRING_CRAP_H

// poooossibly useful stuff for string operations

#include "errorHelpers.h"

// find position of first occurence of char in a string
static inline ssize_t MY_INLINE MY_NON_NULL(1)
	charPos(const char* s, int c) {
		char* loc = strchr(s, c);
		return loc ? loc - s : -1;
	}

extern char* MY_NON_NULL(1)
	removeNewline(char* str);

extern char* MY_NON_NULL(1)
	strReverse(char* str, size_t length);

// TODO: consider returning char* ? xdd
extern void* MY_NON_NULL(1, 2)
	substr(char* dest, char* src, off_t start, size_t length);

extern void* MY_NON_NULL(1)
	removeNinstances(char* str, char c, ssize_t n);

extern void* MY_NON_NULL(1)
	leftShift_fill(void* buff, size_t length, size_t amount, int c);

extern void* MY_NON_NULL(1)
	leftShift(void* buff, size_t length, size_t amount);

extern void* MY_NON_NULL(1)
	leftShift_cycle(void* buff, size_t length, size_t amount);

extern void* MY_NON_NULL(1)
	rightShift_fill(void* buff, size_t length, size_t amount, int c);

extern void* MY_NON_NULL(1)
	rightShift(void* buff, size_t length, size_t amount);

extern void* MY_NON_NULL(1)
	rightShift_cycle(void* buff, size_t length, size_t amount);

#endif // STRING_CRAP_H