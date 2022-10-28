#ifndef ANCIENT_CRAP_H
#define ANCIENT_CRAP_H

// stupid old things go here, don't look

#include <aio.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "errorHelpers.h"

#ifdef HEADER_OKAY

void suspend(struct aiocb* cbp) {
	// TODO: replace with CHECK_RETRY ?
	for (int ret = -1; ret;) {
		// error check except EINTR:
		ERR_NEG1_(ret = aio_suspend((const struct aiocb* const*) &cbp, 1, NULL) , EINTR);
	}

	ERR_NON_ZERO(aio_error(cbp));
	ERR_NEG1(aio_return(cbp));
}

#if RETURN_ERR
#define MY_ERR_CODE -1

#define MIGHT_ERR_(normalType, errType) errType MY_WARN_UNUSED
#define MIGHT_FAIL_(expr, err, ...) err(expr, ##__VA_ARGS__)

#define RETURN_OKAY return 0

#if EXTENSIONS_ALLOWED
#define RETURN_FAIL(reason) (__extension__({\
	errno = EINVAL;\
	DBGonly(reason "\n");\
	return MY_ERR_CODE;\
}))
#else // EXTENSIONS_ALLOWED
#define RETURN_FAIL(reason) do {\
	errno = EINVAL;\
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
	errno = EINVAL;\
	ERR(reason);\
}))
#else // EXTENSIONS_ALLOWED
#define RETURN_FAIL(reason) do {\
	errno = EINVAL;\
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

#endif // HEADER_OKAY
#endif // ANCIENT_CRAP_H