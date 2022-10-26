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

#include <aio.h>
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
//#include <string.h>
#include <sys/socket.h>
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
#include "extraAttr.h"

// smol things
#include "littleHelpers.h"

// error stuff
#include "errorHelpers.h"

// long story short, block comments don't nest nicely lol
#define TEMP_REMOVE 0

// version of DBGprintf that only compiles in DEBUG_MODE
// kinda want DBGprintf to be nice and accesible normally
#if DEBUG_MODE
#define DBGonly(...) DBGprintf(__VA_ARGS__)
#else // DEBUG_MODE
#define DBGonly(...)
#endif // DEBUG_MODE

// bit for functions that sometimes error depending on settings lol
// TODO: generalize RETURN_FAIL? idk
#define MY_ERRNO EINVAL
#if RETURN_ERR
#define MY_ERR_CODE -1

#define MIGHT_ERR_(normalType, errType) errType MY_WARN_UNUSED
#define MIGHT_FAIL_(expr, err, ...) err(expr, ##__VA_ARGS__)

#define RETURN_OKAY return 0

#if EXTENSIONS_ALLOWED
#define RETURN_FAIL(reason) (__extension__({\
	errno = MY_ERRNO;\
	DBGonly(reason "\n");\
	return MY_ERR_CODE;\
}))
#else // EXTENSIONS_ALLOWED
#define RETURN_FAIL(reason) do {\
	errno = MY_ERRNO;\
	DBGonly(reason "\n");\
	return MY_ERR_CODE;\
} while(0)
#endif // EXTENSIONS_ALLOWED

#else // RETURN_ERR
#define MIGHT_ERR_(normalType, errType) normalType
#define MIGHT_FAIL_(expr, err, ...) expr

#define RETURN_OKAY return

#if EXTENSIONS_ALLOWED
#define RETURN_FAIL(reason) (__extension__({\
	errno = MY_ERRNO;\
	ERR(reason);\
}))
#else // EXTENSIONS_ALLOWED
#define RETURN_FAIL(reason) do {\
	errno = MY_ERRNO;\
	ERR(reason);\
} while(0)
#endif // EXTENSIONS_ALLOWED
#endif // RETURN_ERR

// TODO: FIX THIS DUMB MIGHT_ERR STUFF LMFAO
// also add some MIGHT_ERR_CHECK quasi-if thingy
// like NON_ATTR_IF but for MIGHT functions

// more quasi-optional args fiesta eksdi
#define MIGHT_ERR_0_ARGS() MIGHT_ERR_(void, int)
#define MIGHT_ERR_1_ARG(normalType) MIGHT_ERR_(normalType, normalType)
#define MIGHT_ERR_2_ARGS(normalType, errType) MIGHT_ERR_(normalType, errType)
#define MIGHT_ERR_X_ARGS(x, A, B, FUNC, ...) FUNC
#define MIGHT_ERR(...) MIGHT_ERR_X_ARGS(, ##__VA_ARGS__,\
	MIGHT_ERR_2_ARGS(__VA_ARGS__),\
	MIGHT_ERR_1_ARG(__VA_ARGS__),\
	MIGHT_ERR_0_ARGS(__VA_ARGS__))

// So we use these to just fail if we're in RETURN_ERR
// and the function fails? okayyy
// we boutta need a big rework lol
#define MIGHT_FAIL_NEG1(expr, ...) MIGHT_FAIL_(expr, ERR_NEG1, ##__VA_ARGS__)
#define MIGHT_FAIL_NEG(expr, ...) MIGHT_FAIL_(expr, ERR_NEG, ##__VA_ARGS__)
#define MIGHT_FAIL_NON_ZERO(expr, ...) MIGHT_FAIL_(expr, ERR_NON_ZERO, ##__VA_ARGS__)
#define MIGHT_FAIL_EOF(expr, ...) MIGHT_FAIL_(expr, ERR_EOF, ##__VA_ARGS__)
#define MIGHT_FAIL_NULL(expr, ...) MIGHT_FAIL_(expr, ERR_NULL, ##__VA_ARGS__)
#define MIGHT_FAIL_SSIZE_T(expr, ...) MIGHT_FAIL_(expr, ERR_SSIZE_T, ##__VA_ARGS__)
#define MIGHT_FAIL_IN_ADDR_T(expr, ...) MIGHT_FAIL_(expr, ERR_IN_ADDR_T, ##__VA_ARGS__)

// error checked and retried (if EINTR) versions of some message queue functions
#define mq_send_(mqdes, msg_ptr, msg_len, msg_prio, ...)\
	CHECK_RETRY(mq_send(mqdes, msg_ptr, msg_len, msg_prio), ##__VA_ARGS__)
#define mq_receive_(mqdes, msg_ptr, msg_len, msg_prio, ...)\
	CHECK_RETRY(mq_receive(mqdes, msg_ptr, msg_len, msg_prio), ##__VA_ARGS__)
#define mq_close_(mqdes, ...)\
	CHECK_RETRY(mq_close(mqdes), ##__VA_ARGS__)
#define mq_getattr_(mqdes, mqstat, ...)\
	CHECK_RETRY( mq_getattr(mqdes, mqstat), ##__VA_ARGS__)
#define mq_notify_(mqdes, notification, ...)\
	CHECK_RETRY( mq_notify(mqdes, notification), ##__VA_ARGS__)
#define mq_setattr_(mqdes, mqstat, omgstat, ...)\
	CHECK_RETRY( mq_setattr(mqdes, mqstat, omgstat), ##__VA_ARGS__)
#define mq_timedreceive_(mqdes, msg_ptr, msg_len, msg_prio, ...)\
	CHECK_RETRY(mq_timedreveive(mqdes, mqstat, omgstat), ##__VA_ARGS__)
#define mq_timedsend_(mqdes, msg_ptr, msg_len, msg_prio, abstime, ...)\
	CHECK_RETRY(mq_timedsend(mqdes, msg_ptr, msg_len, msg_prio, abstime), ##__VA_ARGS__)
#define mq_unlink_(name, ...)\
	CHECK_RETRY(mq_unlink(name), ##__VA_ARGS__)
// this one's already got __VA_ARGS__ so we can't cleanup the way we do usually
#define mq_open_(name, oflag, ...)\
	CHECK_RETRY_MQ(mq_open(name, oflag, ##__VA_ARGS__))

// error retried and/or checked versions of socket functions:
#define recvfrom_(socket, buffer, length, flags, address, address_len, ...)\
	CHECK_RETRY_SSIZE_T(recvfrom(socket, buffer, length, flags, address, address_len), ##__VA_ARGS__)
#define sendto_(socket, buffer, length, flags, dest_addr, address_len, ...)\
	CHECK_RETRY_SSIZE_T(sendto(socket, buffer, length, flags, dest_addr, address_len), ##__VA_ARGS__)
#define socket_(domain, type, protocol, ...)\
	ERR_NEG1(socket(domain, type, protocol), ##__VA_ARGS__)
#define close_(fd, ...)\
	CHECK_RETRY(close(fd), ##__VA_ARGS__)
#define bind_(socket, address, address_len, ...)\
	ERR_NEG1(bind(socket, address, address_len), ##__VA_ARGS__)
#define listen_(sockfd, backlog, ...)\
	ERR_NEG1(listen(sockfd, backlog), ##__VA_ARGS__)
#define send_(sockfd, buf, len, flags, ...)\
	CHECK_RETRY_SSIZE_T(send(sockfd, buf, len, flags), ##__VA_ARGS__)
#define recv_(socket, buffer, length, flags, ...)\
	CHECK_RETRY_SSIZE_T(recv(socket, buffer, length, flags), ##__VA_ARGS__)
#define accept_(socket, address, address_len, ...)\
	CHECK_RETRY(accept(socket, address, address_len), ##__VA_ARGS__)
#define select_(nfds, readfds, writefds, errorfds, timeout, ...)\
	CHECK_RETRY(select(nfds, readfds, writefds, errorfds, timeout), ##__VA_ARGS__)
#define setsockopt_(socket, level, option_name, option_value, option_len, ...)\
	ERR_NEG1(setsockopt(socket, level, option_name, option_value, option_len), ##__VA_ARGS__)
#define fcntl_(fildes, cmd, ...)\
	CHECK_RETRY(fcntl(fildes, cmd, ##__VA_ARGS__))
#define sigprocmask_(how, set, oset, ...)\
	ERR_NEG1(sigprocmask(how, set, oset), ##__VA_ARGS__)

// I have no idea if these ones need to handle EINTR lol
#define dprintf_(fildes, format, ...)\
	ERR_NEG_(printf(fildes, format, ##__VA_ARGS__))
#define fprintf_(stream, format, ...)\
	ERR_NEG(printf(stream, format, ##__VA_ARGS__))
#define printf_(format, ...)\
	ERR_NEG_(printf(format, ##__VA_ARGS__), EBADF)
// shhh pretend you didn't see that, but also don't remove it b/c everything breaks lol
#define snprintf_(s, n, format, ...)\
	ERR_NEG(snprintf(s, n, format, ##__VA_ARGS__))
#define sprintf_(s, format, ...)\
	ERR_NEG(sprintf(s, format, ##__VA_ARGS__))

#define fflush_(stream, ...)\
	CHECK_RETRY_EOF(fflush(stream), ##__VA_ARGS__)

// lab 3/8-ish stuff?
#define malloc_(size, ...)\
	ERR_NULL(malloc(size), ##__VA_ARGS__)
#define calloc_(nmemb, size, ...)\
	ERR_NULL(calloc(nmemb, size), ##__VA_ARGS__)
#define pthread_create_(thread, attr, start_routine, arg, ...)\
	ERR_NON_ZERO(pthread_create(thread, attr, start_routine, arg), ##__VA_ARGS__)
#define pthread_join_(thread, value_ptr, ...)\
	ERR_NON_ZERO(pthread_join(thread, value_ptr), ##__VA_ARGS__)
#define pthread_mutex_destroy_(mutex, ...)\
	ERR_NON_ZERO(pthread_mutex_destroy(mutex), ##__VA_ARGS__)
#define pthread_mutex_init_(mutex, attr, ...)\
	ERR_NON_ZERO(pthread_mutex_init(mutex, attr), ##__VA_ARGS__)
#define pthread_mutex_lock_(mutex, ...)\
	ERR_NON_ZERO(pthread_mutex_lock(mutex), ##__VA_ARGS__)
#define pthread_mutex_unlock_(mutex, ...)\
	ERR_NON_ZERO(pthread_mutex_unlock(mutex), ##__VA_ARGS__)
#define pthread_mutex_trylock_(mutex, ...)\
	ERR_NON_ZERO(pthread_mutex_trylock(mutex), ##__VA_ARGS__)
#define pthread_detach_(thread, ...)\
	ERR_NON_ZERO(pthread_detach(thread), ##__VA_ARGS__)
#define pthread_attr_init_(attr, ...)\
	ERR_NON_ZERO(pthread_attr_init(attr), ##__VA_ARGS__)
#define pthread_attr_destroy_(attr, ...)\
	ERR_NON_ZERO(pthread_attr_destroy(attr), ##__VA_ARGS__)
#define pthread_attr_getdetachstate_(attr, detachstate, ...)\
	ERR_NON_ZERO(pthread_attr_getdetachstate(attr, detachstate), ##__VA_ARGS__)
#define pthread_attr_setdetachstate_(attr, detachstate, ...)\
	ERR_NON_ZERO(pthread_attr_setdetachstate(attr, detachstate), ##__VA_ARGS__)
#define pthread_sigmask_(how , set, oset, ...)\
	ERR_NON_ZERO(pthread_sigmask(how , set, oset), ##__VA_ARGS__)
#define sigwait_(set, sig, ...)\
	ERR_NON_ZERO(sigwait(set, sig), ##__VA_ARGS__)
#define pthread_cancel_(thread, ...)\
	ERR_NON_ZERO(pthread_cancel(thread), ##__VA_ARGS__)
#define sigaddset_(set, signo, ...)\
	ERR_NEG1(sigaddset(set, signo), ##__VA_ARGS__)
#define pthread_cond_destroy_(cond, ...)\
	ERR_NON_ZERO(pthread_cond_destroy(cond), ##__VA_ARGS__)
#define pthread_cond_init_(cond, attr, ...)\
	ERR_NON_ZERO(pthread_cond_init(cond, attr), ##__VA_ARGS__)
#define pthread_cond_timedwait_(cond, mutex, abstime, ...)\
	ERR_NON_ZERO(pthread_cond_timedwait(cond, mutex, abstime), ##__VA_ARGS__)
#define pthread_cond_wait_(cond, mutex, ...)\
	ERR_NON_ZERO(pthread_cond_wait(cond, mutex), ##__VA_ARGS__)
#define pthread_cond_broadcast_(cond, ...)\
	ERR_NON_ZERO(pthread_cond_broadcast(cond), ##__VA_ARGS__)
#define pthread_cond_signal_(cond, ...)\
	ERR_NON_ZERO(pthread_cond_signal(cond), ##__VA_ARGS__)
#define pthread_barrier_destroy_(barrier, ...)\
	ERR_NON_ZERO(pthread_barrier_destroy(barrier), ##__VA_ARGS__)
#define pthread_barrier_init_(barrier, attr, count, ...)\
	ERR_NON_ZERO(pthread_barrier_init(barrier, attr, count), ##__VA_ARGS__)
#define pthread_barrier_wait_(barrier, ...)\
	ERR_BARRIER(pthread_barrier_wait(barrier), ##__VA_ARGS__)
#define sem_init_(sem, pshared, value, ...)\
	ERR_NEG1(sem_init(sem, pshared, value), ##__VA_ARGS__)
#define sem_open_(name, flag, ...)\
	ERR_SEM(sem_open(name, flag, ##__VA_ARGS__))
#define sem_trywait_(sem, ...)\
	CHECK_RETRY(sem_trywait(sem), ##__VA_ARGS__)
#define sem_wait_(sem, ...)\
	CHECK_RETRY(sem_wait(sem), ##__VA_ARGS__)
#define sem_post_(sem, ...)\
	ERR_NEG1(sem_post(sem), ##__VA_ARGS__)
#define sem_close_(sem, ...)\
	ERR_NEG1(sem_close(sem), ##__VA_ARGS__)
#define sem_destroy_(sem, ...)\
	ERR_NEG1(sem_destroy(sem), ##__VA_ARGS__)

#define pthread_setcancelstate_(state, oldstate, ...)\
	ERR_NEG1(pthread_setcancelstate(state, oldstate), ##__VA_ARGS__)
#define pthread_setcanceltype_(type, oldtype, ...)\
	ERR_NEG1(pthread_setcanceltype(type, oldtype), ##__VA_ARGS__)

// extra:
#define open_(path, oflag, ...)\
	CHECK_RETRY(open(path, oflag, ##__VA_ARGS__))

// TODO: getaddrinfo and stuff lul

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

int MY_NON_NULL(2)
	read_(int fildes, void* buf, size_t nbyte) {
		int returnState = -1
#if DEBUG_MODE
			, i = 0
#endif // DEBUG_MODE
			; 

		do {
			// error check except EINTR:
			ERR_NEG1_(returnState = read(fildes, buf, nbyte), EINTR);
		} while (returnState == -1);

		return returnState;
	}

int MY_NON_NULL(2)
	write_(int fildes, void* buf, size_t nbyte) {
		int returnState = -1
#if DEBUG_MODE
			, i = 0
#endif // DEBUG_MODE
			; 

		do {
			// error check except EINTR:
			ERR_NEG1_(returnState = write(fildes, buf, nbyte), EINTR);
		} while (returnState == -1);

		return returnState;
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

void* MY_NON_NULL(1, 2)
	substr(char* dest, char* src, off_t start, size_t length) {
		// TODO: just remove this lol
		if (
#if !ATTRIBUTES_ALLOWED
				dest == NULL || src == NULL || // wtf??? that's a separte check anyway, no? lmfao
#endif // !ATTRIBUTES_ALLOWED
				dest == src) {
			errno = MY_ERRNO;
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

#define SIGEV_INFO_FORMAT "{notify:%d signo:%d value:{int:%d ptr:%p} funct:%p attr:%p tid:%ld}"

// TODO: this MIGHT_ERR stuff has to go lol
MIGHT_ERR()
	init_notification(struct sigevent* notification, int notify, int signo, int sival_int, void* sival_ptr,
			void (*notify_function) (union sigval), void* notify_attributes) {
		if (sival_int && sival_ptr) {
			// eww
			RETURN_FAIL(LBLUE"sival_int" RESET_ESC" (" DBLUE"arg 4" RESET_ESC") passed "
					LRED_BLD"and" LBLUE" sival_ptr" RESET_ESC" (" DBLUE"arg 5" RESET_ESC") passed.\n"
					"(They are members of a union. Only pass one)");
		}

		if ((SIGEV_THREAD == notify) && !notify_function) {
			RETURN_FAIL(LBLUE"notify" RESET_ESC" (" DBLUE"arg 2" RESET_ESC") is SIGEV_THREAD but "
					LBLUE"notify_function" RESET_ESC" (" DBLUE"arg 6" RESET_ESC") is " LRED_BLD"NULL" RESET_ESC"\n");		
		}

		memset(notification, 0, sizeof(struct sigevent));

		notification->sigev_notify = notify;

		union sigval value;
		if (sival_int)
			value.sival_int = sival_int;
		else
			value.sival_ptr = sival_ptr;

		switch (notify) {
			case SIGEV_NONE:
				{
					RETURN_OKAY;
				}
			case SIGEV_SIGNAL:
				{
					notification->sigev_signo = signo;
					notification->sigev_value = value;
					RETURN_OKAY;
				}
			case SIGEV_THREAD:
				{
					notification->sigev_value = value;
					notification->sigev_notify_function = notify_function;
					notification->sigev_notify_attributes = notify_attributes;
					RETURN_OKAY;
				}
			default:
				{
					RETURN_FAIL(LRED_BLD"invalid " LBLUE"notify" RESET_ESC" (" DBLUE"arg 2" RESET_ESC") passed\n");
				}
		}
	}

struct sigevent MY_WARN_UNUSED
	make_notification(int notify, int signo, int sival_int, void* sival_ptr,
			void (*notify_function) (union sigval), void* notify_attributes) {
		struct sigevent notification;
		MIGHT_FAIL_NEG1(init_notification(&notification, notify, signo, sival_int, sival_ptr,
					notify_function, notify_attributes));
		return notification;
	}
// lol:
#define notification_make(notify, signo, sival_int, sival_ptr, notify_function, notify_attributes)\
	make_notification(notify, signo, sival_int, sival_ptr, notify_function, notify_attributes)

#define AIOCB_INFO_FORMAT "{fildes:%d offs:%ld buf:%p n:%lu prio:%d sigev:" SIGEV_INFO_FORMAT " opcode:%d}"

MIGHT_ERR()
	init_aiocb(struct aiocb* cbp, int fildes, off_t offset, ssize_t nbytes, volatile void* aio_buf,
			struct sigevent* notification) {
		memset(cbp, 0, sizeof(struct aiocb));

		cbp->aio_fildes = fildes;

		if (offset >= 0)
			cbp->aio_offset = offset;
		else {
			RETURN_FAIL(LRED_BLD"negative" RESET_ESC" file " LBLUE"offset" RESET_ESC" (" DBLUE"arg 3"
					RESET_ESC") passed");
		}

		if (aio_buf)
			cbp->aio_buf = aio_buf;
		else ERR_NULL(cbp->aio_buf = malloc(nbytes));

		cbp->aio_nbytes = nbytes;

		if (notification)
			cbp->aio_sigevent = *notification;
		else
			cbp->aio_sigevent.sigev_notify = SIGEV_NONE;

		RETURN_OKAY;
	}

void suspend(struct aiocb* cbp) {
	// TODO: replace with CHECK_RETRY ?
	for (int ret = -1; ret;) {
		// error check except EINTR:
		ERR_NEG1_(ret = aio_suspend((const struct aiocb* const*) &cbp, 1, NULL) , EINTR);
	}

	ERR_NON_ZERO(aio_error(cbp));
	ERR_NEG1(aio_return(cbp));
}

// shouldRead macros
// ?? TODO: change this too lol
#define WRITE false
#define READ true

MIGHT_ERR() MY_NON_NULL(5)
	parallelIO(struct aiocb* aioArr, int fildes, off_t offset, size_t blockSize, void* buf,
			struct sigevent* notification, int parrLevel, bool shouldRead) {
		/*
		 * does an IO operation (either read or write) in parallel, doing parrLevel calls
		 * with freshly initialized (or allocated+init, if not provided) aiocbs
		 * if no notification method is provided, it also suspends until they're all done
		 */

		bool arrNotPassed = false;
		if (!aioArr) {
			if (notification) {
				RETURN_FAIL(LBLUE"notification" RESET_ESC" (" DBLUE"arg 6" RESET_ESC") passed but "
						LBLUE"aioArr" RESET_ESC" (" DBLUE"arg 1" RESET_ESC") is "
						LRED_BLD"NULL" RESET_ESC);
			}

			ERR_NULL(aioArr = malloc(parrLevel * sizeof(struct aiocb)));
			arrNotPassed = true;
		}

		NON_ATTR_IF (!buf) {
			RETURN_FAIL(LBLUE"buf" RESET_ESC" (" DBLUE"arg 5" RESET_ESC") is "
					LRED_BLD"NULL" RESET_ESC"\n");
		}

		/* here buf's length is assumed to be >= blockSize * parrLevel, anything else is undefined behaviour */
		for (int i = 0; i < parrLevel; ++i) {
			off_t bufferOffset = i * blockSize * sizeof(char);
#if RETURN_ERR
			// pretty lazy lmao
			ERR_NEG1(init_aiocb(&aioArr[i], fildes, offset + bufferOffset, blockSize, ((int8_t*) buf) + bufferOffset,
						notification));
#else // RETURN_ERR
			init_aiocb(&aioArr[i], fildes, offset + bufferOffset, blockSize, ((int8_t*) buf) + bufferOffset,
					notification);
#endif // RETURN_ERR
		}

		if (shouldRead)
			for (int i = 0; i < parrLevel; ++i)
				aio_read(&aioArr[i]);
		else
			for (int i = 0; i < parrLevel; ++i)
				aio_write(&aioArr[i]);

		if (!notification)
			for (int i = 0; i < parrLevel; ++i)
				suspend(&aioArr[i]);

		if (arrNotPassed)
			FREE(aioArr);

		RETURN_OKAY;
	}

typedef struct MyNode_ {
	LIST_TYPE val;
	struct MyNode_ *next;
} MyNode;

typedef struct MyList_ {
	MyNode *head,
	       *tail; // okay so let's get real
	// the only real reason to have a tail pointer in a singly
	// linked list is to have fast operations for the ends, but I
	// never used this lol, probably TODO: remove this lol
} MyList;

MyNode* MY_WARN_UNUSED
newMyNode (LIST_TYPE val) {
	MyNode *newMyNode;
	ERR_NULL( newMyNode = malloc(sizeof(MyNode)) );

	newMyNode->val = (LIST_TYPE) val;
	newMyNode->next = NULL;
	return newMyNode;
}

MyList* MY_WARN_UNUSED
newMyList (void) {
	MyList* list;

	// calloc to correctly set NULL pointers
	ERR_NULL( list = calloc(1, sizeof(MyList)) );
	return list;
}

MIGHT_ERR() MY_NON_NULL(1, 2) MY_FORMAT(printf, 2, 0)
	printMyList(const MyList* const list, const char* restrict format) {
		NON_ATTR_IF(!list) {
			RETURN_FAIL("bruh where's the list");
		}

		DBGonly("list:%p format:%s\n[H:%p T:%p] ",
				(void*) list, format,
				(void*) list->head, (void*) list->tail
		       );

		for (MyNode* node = list->head; node; node = node->next) {
			printf_(format, node->val);
		}

		printf_("\n");

		RETURN_OKAY;
	}

size_t MY_NON_NULL(1) MY_WARN_UNUSED
myListLength(const MyList* const list) {
	size_t length = 0;
	for (MyNode* node = list->head; node; node = node->next) {
		++length;
	}

	return length;
}

MyNode* MY_WARN_UNUSED MY_NON_NULL(1)
	popAt(MyList* const list, size_t pos) {
		MyNode *prev = NULL, *node = list->head;

		// is this redundant btw? TODO: remove and check lol
		if (!list->head) {
			return NULL; // Can't delete from empty list
		}

		for (size_t i = 0; i < pos; ++i) {
			if (node) {
				prev = node;
				node = node->next;
			} else {
				return NULL; // Can't delete outside of list
			}
		}

		// idk there's a few SIGSEGVs without this lol
		// it's getting pretty messy now, I should rewrite this whole function tbh
		if (!node) {
			return NULL;
		}

		// If there's a previous node relink, otherwise we're deleting the head node, so update that
		prev ?
			(prev->next = node->next) :
			(list->head = node->next);

		// If we're deleting the tail node, update that
		if (!node->next) {
			list->tail = prev;
		}

		// clean up and return
		node->next = NULL;
		return node;
	}

MyNode* MY_WARN_UNUSED MY_NON_NULL(1)
	peekNodeAt(const MyList* const list, size_t pos) {
		MyNode* node = list->head;

		if (!list->head) {
			return NULL; // peek empty list
		}

		for (size_t i = 0; i < pos; ++i) {
			if (node) {
				node = node->next;
			} else {
				return NULL; // peek outside list
			}
		}

		return node;
	}

LIST_TYPE MY_WARN_UNUSED MY_NON_NULL(1)
	peekValAt(const MyList* const list, size_t pos) {
		MyNode* node;

		// TODO: make this fancy or something lol
		if (NULL == (node = peekNodeAt(list, pos)) ) {
			errno = MY_ERRNO;
			ERR("Can't peek value outside list");
		}

		return node->val;
	}

LIST_TYPE MY_WARN_UNUSED MY_NON_NULL(1)
	popValAt(MyList* const list, size_t pos) {
		MyNode* node;
		LIST_TYPE val;

		if (NULL == (node = popAt(list, pos)) ) {
			errno = MY_ERRNO;
			ERR("Can't pop value outside list"); // ?
		}

		val = node->val;
		FREE(node); // we do a little leakn't

		return val;
	}

LIST_TYPE MY_WARN_UNUSED MY_NON_NULL(1)
	peekFirstVal(const MyList* const list) {
		return peekValAt(list, 0);
	}

LIST_TYPE MY_WARN_UNUSED MY_NON_NULL(1)
	peekLastVal(const MyList* const list) {
		size_t sz = myListLength(list);
		return peekValAt(list, sz - 1);
	}

LIST_TYPE MY_WARN_UNUSED MY_NON_NULL(1)
	popFirstVal(MyList* const list) {
		return popValAt(list, 0);
	}

LIST_TYPE MY_WARN_UNUSED MY_NON_NULL(1)
	popLastVal(MyList* const list) {
		size_t sz = myListLength(list);
		return popValAt(list, sz - 1);
	}

MyNode* MY_WARN_UNUSED MY_NON_NULL(1)
	popLast(MyList* const list) {
		size_t sz = myListLength(list);
		return popAt(list, sz - 1);
	}

MyNode* MY_WARN_UNUSED MY_NON_NULL(1)
	popFirst(MyList* const list) {
		return popAt(list, 0);
	}

MyList* MY_NON_NULL(1)
	clearMyList(MyList* const list) {
		for (MyNode* first; (first = popFirst(list));) {
			FREE(first);
		}

		return list;
	}

void MY_NON_NULL(1)
	deleteMyList (MyList* list) {
		clearMyList(list);
		FREE(list);
	}

MyList* MY_NON_NULL(1)
	deleteAt(MyList* const list, size_t pos) {
		MyNode* node = popAt(list, pos);
		FREE(node);
		return list;
	}

MyList* MY_NON_NULL(1)
	deleteLast(MyList* const list) {
		MyNode* node = popLast(list);
		FREE(node);
		return list;
	}

MyList* MY_NON_NULL(1)
	deleteFirst(MyList* const list) {
		MyNode* node = popAt(list, 0);
		FREE(node);
		return list;
	}

MyList* MY_NON_NULL(1, 3)
	insertAfter(MyList* const list, size_t pos, MyNode* const newMyNode) {
		MyNode *node = list->head;

		for (size_t i = 0; i < pos; ++i) { // Find the node we're inserting after
			if (node && node->next) { // Keep moving if this is a node and there's a next one
				node = node->next;
			} else {
				break; // will by default insert after the last one
			}
		}

		node == list->tail ? (list->tail = newMyNode) : (newMyNode->next = node->next);

		// Be careful in case we insertAfter in an empty list
		!node ? (list->head = newMyNode) : (node->next = newMyNode);
		return list;
	}

MyList* MY_NON_NULL(1, 3)
	insertBefore(MyList* const list, size_t pos, MyNode* const newMyNode) {
		MyNode *prev = NULL, *node = list->head;

		for (size_t i = 0; i < pos; ++i) { // Find the node we're inserting before
			if (node && node->next) {
				prev = node;
				node = node->next;
			} else {
				break; // will by default insert before the last one
			}
		}

		node == list->head ? (list->head = newMyNode) : (prev->next = newMyNode);

		// Be careful in case we insertBefore in an empty list
		!node ? (list->tail = newMyNode) : (newMyNode->next = node);
		return list;
	}

MyList* MY_NON_NULL(1)
	insertValAfter(MyList* const list, size_t pos, LIST_TYPE newVal) {
		// NB: returns list after allocation then insertion
		return insertAfter(list, pos, newMyNode(newVal));
	}

MyList* MY_NON_NULL(1)
	insertValBefore(MyList* const list, size_t pos, LIST_TYPE newVal) {
		return insertBefore(list, pos, newMyNode(newVal));
	}

MyList* MY_NON_NULL(1, 2)
	insertLast(MyList* const list, MyNode* const node) {
		size_t sz = myListLength(list);
		insertAfter(list, sz - 1, node);
		return list;
	}

MyList* MY_NON_NULL(1, 2)
	insertFirst(MyList* const list, MyNode* const node) {
		insertBefore(list, 0, node);
		return list;
	}
MyList* MY_NON_NULL(1)
	insertValLast(MyList* const list, LIST_TYPE newVal) {
		return insertLast(list, newMyNode(newVal));
	}

MyList* MY_NON_NULL(1)
	insertValFirst(MyList* const list, LIST_TYPE newVal) {
		return insertFirst(list, newMyNode(newVal));
	}

// socket stuff will start going below this point

MIGHT_ERR() MY_NON_NULL(1)
	init_sockaddr_in(struct sockaddr_in* addr,
			sa_family_t family, in_port_t port, uint32_t s_addr) {
		NON_ATTR_IF(!addr) {
			RETURN_FAIL("null sockaddr_in struct? bruhhh");
		}

		memset((void*) addr, 0, sizeof(struct sockaddr_in));

		addr->sin_family = family;
		addr->sin_port = port;
		addr->sin_addr.s_addr = s_addr;

		RETURN_OKAY;
	}

struct sockaddr_in MY_WARN_UNUSED
make_sockaddr_in(sa_family_t family, in_port_t port, uint32_t s_addr) {
	struct sockaddr_in addr;
	MIGHT_FAIL_NEG1(init_sockaddr_in(&addr, family, port, s_addr));

	return addr;
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

void sethandler(void (*signalHandler) (int), int sigNo) {
	struct sigaction action = {0};
	action.sa_handler = signalHandler;
	ERR_NEG1( sigaction(sigNo, &action, NULL) ); // TODO: change this
}

sigset_t MY_WARN_UNUSED
make_sigset_t_(int signo) {
	sigset_t sigset;
	ERR_NEG1(sigemptyset(&sigset));

	// to allow returning emptyset if signo == 0
	if (signo) {
		sigaddset_(&sigset, signo);
	}

	return sigset;
}
// bigbrane """vararg""" lul
#define make_sigset_t_0_ARGS() make_sigset_t_(0)
#define make_sigset_t_1_ARG(signo) make_sigset_t_(signo)
#define make_sigset_t_X_ARGS(x, A, FUNC, ...) FUNC
#define make_sigset_t(...) make_sigset_t_X_ARGS(, ##__VA_ARGS__,\
		make_sigset_t_1_ARG(__VA_ARGS__),\
		make_sigset_t_0_ARGS(__VA_ARGS__))

#define sigset_make(...) make_sigset_t(__VA_ARGS__)
// gonna start adding ^these b/c the POSIX ones look like this
// will keep the old format too tho b/c I have some old programs with them :(
// we do sum backwards compatibility ig

pthread_mutex_t MY_WARN_UNUSED
make_pthread_mutex_t_(const pthread_mutexattr_t* attr) {
	pthread_mutex_t mutex;
	pthread_mutex_init_(&mutex, attr);
	return mutex;
}
#define make_pthread_mutex_t_0_ARGS() make_pthread_mutex_t_(NULL)
#define make_pthread_mutex_t_1_ARG(attr) make_pthread_mutex_t_(attr)
#define make_pthread_mutex_t_X_ARGS(x, A, FUNC, ...) FUNC
#define make_pthread_mutex_t(...) make_pthread_mutex_t_X_ARGS(, ##__VA_ARGS__,\
		make_pthread_mutex_t_1_ARG(__VA_ARGS__),\
		make_pthread_mutex_t_0_ARGS(__VA_ARGS__))
#define pthread_mutex_make(...) make_pthread_mutex_t(__VA_ARGS__)

pthread_attr_t MY_WARN_UNUSED
make_pthread_attr_t(void) {
	pthread_attr_t attr;
	pthread_attr_init_(&attr);
	return attr;
}
#define pthread_attr_make() make_pthread_attr_t()

pthread_cond_t MY_WARN_UNUSED
make_pthread_cond_t_(const pthread_condattr_t* attr) {
	pthread_cond_t cond;
	pthread_cond_init_(&cond, attr);
	return cond;
}
#define make_pthread_cond_t_0_ARGS() make_pthread_cond_t_(NULL)
#define make_pthread_cond_t_1_ARG(attr) make_pthread_cond_t_(attr)
#define make_pthread_cond_t_X_ARGS(x, A, FUNC, ...) FUNC
#define make_pthread_cond_t(...) make_pthread_cond_t_X_ARGS(, ##__VA_ARGS__,\
		make_pthread_cond_t_1_ARG(__VA_ARGS__),\
		make_pthread_cond_t_0_ARGS(__VA_ARGS__))
#define pthread_cond_make(...) make_pthread_cond_t(__VA_ARGS__)

pthread_barrier_t MY_WARN_UNUSED
make_pthread_barrier_t_(const pthread_barrierattr_t* attr, unsigned count) {
	pthread_barrier_t barrier;
	pthread_barrier_init_(&barrier, attr, count);
	return barrier;
}
#define make_pthread_barrier_t_0_ARGS() make_pthread_barrier_t_(NULL, 2)
#define make_pthread_barrier_t_1_ARG(count) make_pthread_barrier_t_(NULL, count)
#define make_pthread_barrier_t_2_ARGS(attr, count) make_pthread_barrier_t_(attr, count)
#define make_pthread_barrier_t_X_ARGS(x, A, B, FUNC, ...) FUNC
#define make_pthread_barrier_t(...) make_pthread_barrier_t_X_ARGS(, ##__VA_ARGS__,\
	make_pthread_barrier_t_2_ARGS(__VA_ARGS__),\
	make_pthread_barrier_t_1_ARG(__VA_ARGS__),\
	make_pthread_barrier_t_0_ARGS(__VA_ARGS__))
#define pthread_barrier_make(...) make_pthread_barrier_t(__VA_ARGS__)

sem_t MY_WARN_UNUSED
make_sem_t_(int pshared, unsigned value) {
	sem_t sem;
	sem_init_(&sem, pshared, value);
	return sem;
}
#define make_sem_t_0_ARGS() make_sem_t_(0, 1) /* binary sem by default ig lol */
#define make_sem_t_1_ARG(count) make_sem_t_(0, count)
#define make_sem_t_2_ARGS(pshared, count) make_sem_t_(pshared, count)
#define make_sem_t_X_ARGS(x, A, B, FUNC, ...) FUNC
#define make_sem_t(...) make_sem_t_X_ARGS(, ##__VA_ARGS__,\
	make_sem_t_2_ARGS(__VA_ARGS__),\
	make_sem_t_1_ARG(__VA_ARGS__),\
	make_sem_t_0_ARGS(__VA_ARGS__))
#define sem_make(...) make_sem_t(__VA_ARGS__)

#endif // HEADER_OKAY
#undef HEADER_OKAY

#undef DEBUG_MODE
#undef RETURN_ERR
//#undef MY_ERRNO
//#undef MY_ERR_CODE
#endif // KATWIK_HEADER
