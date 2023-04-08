#ifndef TIMERFD_HELPERS_H
#define TIMERFD_HELPERS_H

#include <sys/timerfd.h>

#define timerfd_create_(clockid, flags)\
	ERR_NEG1(timerfd_create(clockid, flags))

#define timerfd_settime_(fd, flags, new_value, old_value)\
	ERR_NEG1(timerfd_settime(fd, flags, new_value, old_value))

#define timerfd_gettime_(fd, curr_value)\
	ERR_NEG1(timerfd_gettime(fd, curr_value))

#endif // TIMERFD_HELPERS_H
