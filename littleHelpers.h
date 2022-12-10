#ifndef LITTLE_HELPERS_H
#define LITTLE_HELPERS_H

#include <sys/wait.h>

#include "easyCheck.h"

extern void waitAllChildren(void);

extern void printBuf(char* buffer, size_t sz);

// easy way to set a signal handler AND error check
extern void sethandler(void (*signalHandler) (int), int sigNo);

// world's most overkill swap? xdd
// can swap structs and can use an optionally given buffer, meh?
extern void MY_NON_NULL(1, 2)
	swap_(void* restrict lhs, void* restrict rhs, size_t sz, void* restrict buffer);
#define swap_3_ARGS(lhs, rhs, sz) swap_(lhs, rhs, sz, NULL)
#define swap_4_ARGS(lhs, rhs, sz, buffer) swap_(lhs, rhs, sz, buffer)
#define swap_X_ARGS(x, A, B, C, D, FUNC, ...) FUNC
#define swap(...) swap_X_ARGS(, ##__VA_ARGS__,\
	swap_4_ARGS(__VA_ARGS__),\
	swap_3_ARGS(__VA_ARGS__),\
	swap_2_ARGS(__VA_ARGS__),/* Wimplicit-function-declaration for rest */\
	swap_1_ARG(__VA_ARGS__),\
	swap_0_ARGS(__VA_ARGS__))

#endif // LITTLE_HELPERS_H