/*!
 * @file katwikOpsys.h
 * @brief header with stuff
 * @author Emmanuel K
 */
#ifndef KATWIK_HEADER
#define KATWIK_HEADER

#ifndef __GNUC__
#error bruhhhh moment no GNU?
#undef HEADER_OKAY
#endif // __GNUC__

#define _POSIX_C_SOURCE 200809L

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

// this one can actually be removed b/c
// no actual code requires it,
// buuut there's some macros of this header's functions
// so yeahh I guess this makes life easier sometimes?
#include <mqueue.h>

// settings n shiet for le header
#include "options.h"

// rest of header will now be invisible after the preprocesser if the first section wasn't set
// up properly, to reduce the flooding of errors from the remaining section being invalid as a consequence.
// in some cases this will in exchange increase "Wimplicit-function-declaration" and dumb linker errors,
// but overall it should make stuff cleaner for new users who haven't yet tried to use anything from
// it, but are confused on why it "doesn't just work" (you know who you are xD)
#ifndef HEADER_OKAY
#pragma message "Some macro wasn't set up properly, rest of header won't be preprocessed"
#else

// cute colorz
#include "colors.h"

// extra attributes
#include "easyAttr.h"

// smol things
#include "littleHelpers.h"

// error stuff
#include "errorHelpers.h"

// functions that are already error checked
#include "easyCheck.h"

// some string operations, mayyybe useful
#include "stringCrap.h"

// homebrew linked list xdd
#include "myList.h"

#include "ancientCrap.h"

void waitAllChildren(void) {
#if DEBUG_MODE
	int i = 0;
#endif // DEBUG_MODE

	for (pid_t pid; true;) {
		// error check except ECHILD:
		ERR_NEG1_(pid = waitpid(-1, NULL, 0) , ECHILD);
		if (errno == ECHILD) break;
	}
}

// TODO: check handling of EINTR here
void myNanoSleep (long nanoseconds) {
	/*
	 * nanosleeps for the given nanoseconds
	 */

	struct timespec time;
	time.tv_sec = nanoseconds / GIGA; // int division
	time.tv_nsec = nanoseconds % GIGA;

	for (int returnState = -1
#if DEBUG_MODE
			, i = 0
#endif // DEBUG_MODE
			; returnState;) {
		// error check except EINTR:
		ERR_NEG1_(returnState = nanosleep(&time, &time) , EINTR);
	}
}

// TODO: check handling of EINTR here
void myNanoSleep2(time_t seconds, long nanoseconds) {
	DBGonly("s:%ld ns:%ld\n",
			seconds, nanoseconds
	       );

	struct timespec time;
	time.tv_sec = seconds;
	time.tv_nsec = nanoseconds;

	for (int returnState = -1
#if DEBUG_MODE
			, i = 0
#endif // DEBUG_MODE
			; returnState;) {
		// error check except EINTR:
		ERR_NEG1_(returnState = nanosleep(&time, &time) , EINTR);
	}
}

// TODO: is ulong truly necessary? ...
// TODO: add more randomizers also?
unsigned long MY_WARN_UNUSED
#if USE_RAND_R
MY_NON_NULL(3)
#endif // USE_RAND_R
	myRand(unsigned long min, unsigned long max
#if USE_RAND_R
			, unsigned int* seed
#endif // USE_RAND_R
	      ) {
		// TODO: add seed check for if attributes aren't allowed

		return min +
#if USE_RAND_R
			rand_r(seed)
#else // USE_RAND_R
			rand()
#endif // USE_RAND_R
			% (max - min + 1);
	}

double MY_WARN_UNUSED
#if USE_RAND_R
MY_NON_NULL(3)
#endif // USE_RAND_R
	myRandDouble(double min, double max
#if USE_RAND_R
			, unsigned int* seed
#endif // USE_RAND_R
		    ) {

		return min + (
#if USE_RAND_R
				myRand(0, GIGA, seed)
#else // USE_RAND_R
				myRand(0, GIGA)
#endif // USE_RAND_R
				* 1.0L / GIGA) * (max - min);
	}

void
#if USE_RAND_R
MY_NON_NULL(3)
#endif // USE_RAND_R
	myRandSleep(double minSec, double maxSec
#if USE_RAND_R
			, unsigned int* seed
#endif // USE_RAND_R
		   ) {
		myNanoSleep(myRand(minSec * GIGA, maxSec * GIGA
#if USE_RAND_R
					, seed
#endif // USE_RAND_R
				  ));
	}

void printBuf(char* buffer, size_t sz) {
	write_(STDOUT_FILENO, buffer, sz);
	printf_("\n");
}

// should be might_err return type
in_addr_t MY_NON_NULL(1)
	inet_addr_(const char* cp) {
		NON_ATTR_IF(!addr) {
			RETURN_FAIL("null string? bruhhh");
		}

		in_addr_t addr;
		// should be a different macro
		ERR_IN_ADDR_T(addr = inet_addr(cp));
		return addr;
	}

// easy way to set a signal handler AND error check
void sethandler(void (*signalHandler) (int), int sigNo) {
	struct sigaction action = {0};
	action.sa_handler = signalHandler;
	ERR_NEG1( sigaction(sigNo, &action, NULL) ); // TODO: change this
}

#endif // HEADER_OKAY
#undef HEADER_OKAY

#undef DEBUG_MODE
#undef RETURN_ERR
//#undef MY_ERR_CODE
#endif // KATWIK_HEADER
