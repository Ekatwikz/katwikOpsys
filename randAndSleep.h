#ifndef RAND_AND_SLEEP_H
#define RAND_AND_SLEEP_H

#include <time.h>

#include "easyAttr.h"
#include "errorHelpers.h"

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
#endif // USE_RAND_R

extern void myNanoSleep (long nanoseconds);

extern void myNanoSleep2(time_t seconds, long nanoseconds);

extern unsigned long MY_WARN_UNUSED
#if USE_RAND_R
MY_NON_NULL(3)
#endif // USE_RAND_R
	myRand(unsigned long min, unsigned long max
#if USE_RAND_R
			, unsigned int* seed
#endif // USE_RAND_R
	      );

extern double MY_WARN_UNUSED
#if USE_RAND_R
MY_NON_NULL(3)
#endif // USE_RAND_R
	myRandDouble(double min, double max
#if USE_RAND_R
			, unsigned int* seed
#endif // USE_RAND_R
		    );

extern void
#if USE_RAND_R
MY_NON_NULL(3)
#endif // USE_RAND_R
	myRandSleep(double minSec, double maxSec
#if USE_RAND_R
			, unsigned int* seed
#endif // USE_RAND_R
		   );

#endif // RAND_AND_SLEEP_H