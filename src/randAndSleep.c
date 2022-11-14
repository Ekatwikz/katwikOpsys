#include "randAndSleep.h"

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