#ifndef LITTLE_HELPERS_H
#define LITTLE_HELPERS_H

#include <sys/wait.h>

#include "easyCheck.h"

extern void waitAllChildren(void);

extern void printBuf(char* buffer, size_t sz);

// easy way to set a signal handler AND error check
extern void sethandler(void (*signalHandler) (int), int sigNo);

#endif // LITTLE_HELPERS_H