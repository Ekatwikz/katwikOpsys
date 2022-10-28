#ifndef LITTLE_HELPERS_H
#define LITTLE_HELPERS_H

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "easyAttr.h"

// big number
#define GIGA 1000000000L

// long story short, block comments don't nest nicely lol
#define TEMP_REMOVE 0

// useful to force stringify some stuff
// 2 layers because uhhhh umm
// something something stackoverflow
#define TO_STR_(s) #s
#define TO_STR(X) TO_STR_(X)

// dodges ASAN sometimes :)
// a similar macro should be in safe-malloc.h but I can't include that for some reason :(
#define FREE(ptr) do {\
	free(ptr);\
	(ptr) = NULL;\
} while(0)
	
// goodbye Wunused-parameter/Wunused-variable lol
#define UNUSED(thing) do {\
	(void) (thing);\
} while(0)

// find position of first occurence of char in a string
static inline ssize_t MY_INLINE MY_NON_NULL(1)
	charPos(const char* s, int c) {
		char* loc = strchr(s, c);
		return loc ? loc - s : -1;
	}

#endif // LITTLE_HELPERS_H