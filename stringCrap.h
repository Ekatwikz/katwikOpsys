#ifndef STRING_CRAP_H
#define STRING_CRAP_H

// poooossibly useful stuff for string operations

#include "errorHelpers.h"
// TODO: consider returning char* ? xdd

#ifdef HEADER_OKAY

// find position of first occurence of char in a string
static inline ssize_t MY_INLINE MY_NON_NULL(1)
	charPos(const char* s, int c) {
		char* loc = strchr(s, c);
		return loc ? loc - s : -1;
	}

void* MY_NON_NULL(1, 2)
	substr(char* dest, char* src, off_t start, size_t length) {
		// TODO: just remove this lol
		if (
#if !ATTRIBUTES_ALLOWED
				dest == NULL || src == NULL || // wtf??? that's a separte check anyway, no? lmfao
#endif // !ATTRIBUTES_ALLOWED
				dest == src) {
			errno = EINVAL;
			ERR(LBLUE"dest" RESET_ESC" (" DBLUE"arg 1" RESET_ESC") "
					LRED_BLD"==" LBLUE" src" RESET_ESC" (" DBLUE"arg 2" RESET_ESC")");
		}

		size_t i = 0;

		// TODO: holy shit rework this to use memmove/memcopy lmfao
		// why actually do the work?
		for (; i < length;
				dest[i] = src[start + i],
				++i);

		dest[i] = '\0';

		return dest;
	}

void* MY_NON_NULL(1)
	leftShift_fill(void* buff, size_t length, size_t amount, int c) {
		memmove(buff, ((int8_t*) buff) + amount, length - amount);
		memset(((int8_t*) buff) + length - amount, c, amount);

		return buff;
	}

// TODO: debug-mode the rest of this stuff
void* MY_NON_NULL(1)
	leftShift(void* buff, size_t length, size_t amount) {
		return leftShift_fill(buff, length, amount, 0);
	}

void* MY_NON_NULL(1)
	leftShift_cycle(void* buff, size_t length, size_t amount) {
		void* tmp;
		ERR_NULL(tmp = malloc(amount));
		memcpy(tmp, buff, amount);

		// leftShift(buff, length, amount);
		memmove(buff, ((int8_t*) buff) + amount, length - amount);

		memcpy(((int8_t*) buff) + length - amount, tmp, amount);
		FREE(tmp);

		return buff;
	}

void* MY_NON_NULL(1)
	rightShift_fill(void* buff, size_t length, size_t amount, int c) {
		return memset(((int8_t*) buff) + length - amount, c, amount);
	}

void* MY_NON_NULL(1)
	rightShift(void* buff, size_t length, size_t amount) {
		return rightShift_fill(buff, length, amount, 0);
	}

void* MY_NON_NULL(1)
	rightShift_cycle(void* buff, size_t length, size_t amount) {
		void* tmp;
		ERR_NULL(tmp = malloc(amount));
		memcpy(tmp, ((int8_t*) buff) + length - amount, amount);
		memmove(&((int8_t*) buff)[amount], buff, length - amount);
		memcpy(buff, tmp, amount);
		FREE(tmp);
		return buff;
	}

void* MY_NON_NULL(1)
	removeNinstances(char* str, char c, ssize_t n) {
		size_t sz = strlen(str);
		for (size_t i = 0; i < sz && n; ++i)
			for (; str[i] == c; --n)
				leftShift(str + i, sz - i, 1);
		return str;
	}

char* MY_NON_NULL(1)
	removeNewline(char* str) {
		// keep a copy to return? idk
		char* ret = str;
		for (; *str; ++str) { // lol
			if ('\n' == *str && '\0' == *(str + 1)) {
				*str = '\0';
				break;
			}
		}

		return ret;
	}

#endif // STRING_CRAP_H

#endif // HEADER_OKAY