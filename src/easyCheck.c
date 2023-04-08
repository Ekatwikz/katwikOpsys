#include "easyCheck.h"

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

pthread_mutex_t MY_WARN_UNUSED
make_pthread_mutex_t_(const pthread_mutexattr_t* attr) {
	pthread_mutex_t mutex;
	pthread_mutex_init_(&mutex, attr);
	return mutex;
}

pthread_attr_t MY_WARN_UNUSED
make_pthread_attr_t(void) {
	pthread_attr_t attr;
	pthread_attr_init_(&attr);
	return attr;
}

pthread_cond_t MY_WARN_UNUSED
make_pthread_cond_t_(const pthread_condattr_t* attr) {
	pthread_cond_t cond;
	pthread_cond_init_(&cond, attr);
	return cond;
}

pthread_barrier_t MY_WARN_UNUSED
make_pthread_barrier_t_(const pthread_barrierattr_t* attr, unsigned count) {
	pthread_barrier_t barrier;
	pthread_barrier_init_(&barrier, attr, count);
	return barrier;
}

sem_t MY_WARN_UNUSED
make_sem_t_(int pshared, unsigned value) {
	sem_t sem;
	sem_init_(&sem, pshared, value);
	return sem;
}

int MY_NON_NULL(2)
	read_(int fildes, void* buf, size_t nbyte) {
		int returnState;

		do {
			// error check except EINTR:
			ERR_NEG1_(returnState = read(fildes, buf, nbyte), EINTR);
		} while (returnState == -1);

		return returnState;
	}

int MY_NON_NULL(2)
	write_(int fildes, void* buf, size_t nbyte) {
		int returnState;

		do {
			// error check except EINTR:
			ERR_NEG1_(returnState = write(fildes, buf, nbyte), EINTR);
		} while (returnState == -1);

		return returnState;
	}