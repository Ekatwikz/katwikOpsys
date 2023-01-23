#ifndef AIOHELPERS_H
#define AIOHELPERS_H

#include <aio.h>
#include "errorHelpers.h"

void suspend(struct aiocb* cbp) {
	CHECK_RETRY_(aio_suspend((const struct aiocb* const*) &cbp, 1, NULL) , EINTR);
	ERR_NON_ZERO(aio_error(cbp));
	ERR_NEG1(aio_return(cbp));
}

int MY_WARN_UNUSED
init_notification(struct sigevent* notification, int notify, int signo, int sival_int, void* sival_ptr,
		void (*notify_function) (union sigval), void* notify_attributes) {
	if (sival_int && sival_ptr) {
		DBGprintf(LBLUE"sival_int" RESET_ESC" (" DBLUE"arg 4" RESET_ESC") passed "
				LRED_BLD"and" LBLUE" sival_ptr" RESET_ESC" (" DBLUE"arg 5" RESET_ESC") passed.\n"
				"(They are members of a union. Only pass one)");
		return -1;
	}

	if ((SIGEV_THREAD == notify) && !notify_function) {
		DBGprintf(LBLUE"notify" RESET_ESC" (" DBLUE"arg 2" RESET_ESC") is SIGEV_THREAD but "
				LBLUE"notify_function" RESET_ESC" (" DBLUE"arg 6" RESET_ESC") is " LRED_BLD"NULL" RESET_ESC"\n");
		return -1;
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
				return 0;
			}
		case SIGEV_SIGNAL:
			{
				notification->sigev_signo = signo;
				notification->sigev_value = value;
				return 0;
			}
		case SIGEV_THREAD:
			{
				notification->sigev_value = value;
				notification->sigev_notify_function = notify_function;
				notification->sigev_notify_attributes = notify_attributes;
				return 0;
			}
		default:
			{
				DBGprintf(LRED_BLD"invalid " LBLUE"notify" RESET_ESC" (" DBLUE"arg 2" RESET_ESC") passed\n");
				return -1;
			}
	}
}

struct sigevent MY_WARN_UNUSED
make_notification(int notify, int signo, int sival_int, void* sival_ptr,
		void (*notify_function) (union sigval), void* notify_attributes) {
	struct sigevent notification;
	ERR_NEG1(init_notification(&notification, notify, signo, sival_int, sival_ptr,
				notify_function, notify_attributes));
	return notification;
}
// lol:
#define notification_make(notify, signo, sival_int, sival_ptr, notify_function, notify_attributes)\
	make_notification(notify, signo, sival_int, sival_ptr, notify_function, notify_attributes)

int MY_WARN_UNUSED MY_NON_NULL(1)
	init_aiocb(struct aiocb* cbp, int fildes, off_t offset, ssize_t nbytes, volatile void* aio_buf,
			struct sigevent* notification) {
		memset(cbp, 0, sizeof(struct aiocb));

		cbp->aio_fildes = fildes;

		if (offset >= 0)
			cbp->aio_offset = offset;
		else {
			DBGprintf(LRED_BLD"negative" RESET_ESC" file " LBLUE"offset" RESET_ESC" (" DBLUE"arg 3"
					RESET_ESC") passed\n");
			return -1;
		}

		if (aio_buf)
			cbp->aio_buf = aio_buf;
		else
			ERR_NULL(cbp->aio_buf = malloc(nbytes));

		cbp->aio_nbytes = nbytes;

		if (notification)
			cbp->aio_sigevent = *notification;
		else
			cbp->aio_sigevent.sigev_notify = SIGEV_NONE;

		return 0;
	}

struct aiocb MY_WARN_UNUSED
make_aiocb(int fildes, off_t offset, ssize_t nbytes, volatile void* aio_buf,
		struct sigevent* notification) {
	struct aiocb aiocb;
	ERR_NEG1(init_aiocb(&aiocb, fildes, offset, nbytes, aio_buf, notification));
	return aiocb;
}
#define aiocb_make(fildes, offset, nbytes, aio_buf, notification)\
	make_aiocb(fildes, offset, nbytes, aio_buf, notification)

// shouldRead macros
// ?? TODO: change this too lol
#define WRITE false
#define READ true
/*
 * (ancient, don't use)
 * does an IO operation (either read or write) in parallel, doing parrLevel calls
 * with freshly initialized (or allocated+init, if not provided) aiocbs
 * if no notification method is provided, it also suspends until they're all done
 */
int MY_WARN_UNUSED MY_DEPRECATE("Bruh dis ting wack") MY_NON_NULL(5)
	parallelIO(struct aiocb* aioArr, int fildes, off_t offset, size_t blockSize, void* buf,
			struct sigevent* notification, int parrLevel, bool shouldRead) {
		bool arrNotPassed = false;
		if (!aioArr) {
			if (notification) {
				DBGprintf(LBLUE"notification" RESET_ESC" (" DBLUE"arg 6" RESET_ESC") passed but "
						LBLUE"aioArr" RESET_ESC" (" DBLUE"arg 1" RESET_ESC") is "
						LRED_BLD"NULL" RESET_ESC);
				return -1;
			}

			ERR_NULL(aioArr = malloc(parrLevel * sizeof(struct aiocb)));
			arrNotPassed = true;
		}

		NON_ATTR_IF (!buf) {
			DBGprintf(LBLUE"buf" RESET_ESC" (" DBLUE"arg 5" RESET_ESC") is "
					LRED_BLD"NULL" RESET_ESC"\n");
			return -1;
		}

		/* here buf's length is assumed to be >= blockSize * parrLevel, anything else is undefined behaviour */
		for (int i = 0; i < parrLevel; ++i) {
			off_t bufferOffset = i * blockSize * sizeof(char);
			int init_ret = init_aiocb(&aioArr[i], fildes, offset + bufferOffset, blockSize, ((int8_t*) buf) + bufferOffset,
					notification);

			if (init_ret == -1) {
				if (arrNotPassed) {
					FREE(aioArr);
				}
				return init_ret;
			}
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

		return 0;
	}
#define parallelIO_(aioArr, fildes, offset, blockSize, buf, notification, parrLevel, shouldRead)\
	ERR_NEG1(parallelIO(aioArr, fildes, offset, blockSize, buf, notification, parrLevel, shouldRead))

#endif // AIOHELPERS_H

