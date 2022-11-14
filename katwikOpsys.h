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

#include <arpa/inet.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>

// settings n stuff for le header
#include "options.h"

// 1 to use rand_r in randomizers
#ifndef USE_RAND_R
#define USE_RAND_R 0
#elif USE_RAND_R == 1
#if MUNDANE_MESSAGES
#pragma message "Usage of rand_r has been manully turned on, you will have to pass a seed to all randomizers"
#endif // MUNDANE_MESSAGES
#elif USE_RAND_R == 0
#if MUNDANE_MESSAGES
#pragma message "Usage of rand_r has been manully turned off"
#endif // MUNDANE_MESSAGES
#else
#pragma GCC error "USE_RAND_R macro bad value.\nValid values are 0 (off) or 1 (on)"
#undef HEADER_OKAY
#endif // USE_RAND_R

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

// stuff to kill off
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

// easy way to set a signal handler AND error check
void sethandler(void (*signalHandler) (int), int sigNo) {
	struct sigaction action = {0};
	action.sa_handler = signalHandler;
	ERR_NEG1( sigaction(sigNo, &action, NULL) ); // TODO: change this
}

#undef DEBUG_MODE
//#undef MY_ERR_CODE
#endif // KATWIK_HEADER
