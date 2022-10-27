#ifndef EASYCHECK_H
#define EASYCHECK_H

#include <netinet/in.h>
#include <semaphore.h>

#include "errorHelpers.h"
#include "options.h"

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

#endif // EASYCHECK_H