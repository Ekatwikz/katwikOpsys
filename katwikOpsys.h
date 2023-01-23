#ifndef KATWIK_OPSYS_H
#define KATWIK_OPSYS_H

/*!
 * @file katwikOpsys.h
 * @brief header with stuff
 * @author Emmanuel K
 */
#ifndef __GNUC__
#error bruhhhh moment no GNU?
#endif // __GNUC__

#include <arpa/inet.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>

// assorted useful stuff
#include "littleHelpers.h"

// some string operations, maybe useful
#include "stringCrap.h"

// homebrew linked list xdd
#include "myList.h"

// randomizer, sleepers and random-sleepers lol
#include "randAndSleep.h"

// aio stuff
#include "aioHelpers.h"

// net stuff
#include "netHelpers.h"

#endif // KATWIK_OPSYS_H
