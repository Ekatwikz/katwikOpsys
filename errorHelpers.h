#ifndef ERROR_HELPERS_H
#define ERROR_HELPERS_H

#include <errno.h>
#include <execinfo.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "options.h"
#include "colors.h"
#include "easyAttr.h"
#include "tinyHelpers.h"

#ifndef ERR_MULTIPROCESS
#pragma GCC warning "ERR_MULTIPROCESS macro undefined, will default to 0\nSet it in your makefile.\nFor multi-process programs:\nERR_MULTIPROCESS=1\nFor single-process programs:\nRR_MULTIPROCESS=0"
#define ERR_MULTIPROCESS 0
#elif ERR_MULTIPROCESS != 1 && ERR_MULTIPROCESS != 0
#pragma GCC warning "ERR_MULTIPROCESS invalid value, will default to 0\nFor multi-process programs:\n#define ERR_MULTIPROCESS 1\nFor single-process programs:\n#define ERR_MULTIPROCESS 0"
#define ERR_MULTIPROCESS 0
#endif // ERR_MULTIPROCESS

#ifndef TRACE_ON_ERR
#define TRACE_ON_ERR 1
#elif TRACE_ON_ERR == 1
#if MUNDANE_MESSAGES
#pragma message "Tracing when ERR happens has been manually enabled"
#endif // MUNDANE_MESSAGES
#elif TRACE_ON_ERR == 0
#if MUNDANE_MESSAGES
#pragma message "Tracing when ERR happens has been manually disallowed"
#endif // MUNDANE_MESSAGES
#else
#pragma GCC error "TRACE_ON_ERR macro bad value.\nValid values are 1 (allowed) or 0 (disallowed)"
#undef HEADER_OKAY
#endif // TRACE_ON_ERR

#ifndef EXEC_ALLOWED
#define EXEC_ALLOWED 1
#elif EXEC_ALLOWED == 1
#if MUNDANE_MESSAGES
#pragma message "Header's usage of exec (and similar functions) has been manually allowed"
#endif // MUNDANE_MESSAGES
#elif EXEC_ALLOWED == 0
#if MUNDANE_MESSAGES
#pragma message "Header's usage of exec (and similar functions) has been manually disallowed"
#endif // MUNDANE_MESSAGES
#else
#pragma GCC error "EXEC_ALLOWED macro bad value, will default to 0\nValid values are 1 (allowed) or 0 (disallowed)"
#define EXEC_ALLOWED 0
#undef HEADER_OKAY
#endif // EXEC_ALLOWED

//  requires:
//  __FILE__, __LINE__, __func__, pid, ppid, tid
#define INFO_FORMAT GREEN_BLD"%s" RESET_ESC":" YELLLOW"%d"\
		RESET_ESC" in " VIOLET_BLD"%s" RESET_ESC", PID:"\
		BWHITE"%06ld" RESET_ESC" PPID:" BWHITE"%06ld"\
		RESET_ESC" TID:" BWHITE"%ld" RESET_ESC

#if EXEC_ALLOWED
#define MAX_EXEC_STR 1000
#define MAX_EXEC_LINE 200
#define EXEC_ONLY if (1)
#else // EXEC_ALLOWED
#define MAX_EXEC_STR 0
#define MAX_EXEC_LINE 0
#define EXEC_ONLY if (0)
#endif // EXEC_ALLOWED

// NB: isMultiprocess is known at compile time
// so any regular compiler won't even assemble
// the if statement
#define ERR_EXIT(isMultiprocess) do {\
	if (isMultiprocess) kill(0, SIGKILL); /* Process Group */\
	else exit(EXIT_FAILURE); /* Own Process Only */\
} while(0)

extern void myStackTracer(FILE* stream,
	int oflag // possible options for oflag bitfield
#define TRUNCATE_RESERVED 01
#define TRUNCATE_TRACE_FUNCTIONS 02
#define TRUNCATE_UNKNOWN 04
#define TRUNCATE_ALOT TRUNCATE_RESERVED | TRUNCATE_TRACE_FUNCTIONS | TRUNCATE_UNKNOWN
);

#if TRACE_ON_ERR
#define ERR_STACK_TRACE() myStackTracer(stderr, TRUNCATE_ALOT); fprintf(stderr, "\n")
#else // TRACE_ON_ERR
#define ERR_STACK_TRACE()
#endif // TRACE_ON_ERR

// NB: the manuals state that getpid/getppid are always successful and no errors are reserved
// Also this version can run some optional extra code (usually cleanup) before exiting
#define ERR_(source, cleanup) do {\
	int originalErrno = errno;/* just in case something weird happens lmao */\
	\
	close(STDOUT_FILENO); /* sometimes some weeeird stuff happens without this lol*/\
	/* especially in multithreaded environments */\
	\
	fprintf(stderr,\
			RED_BLD_INV"errno=%d" RESET_ESC" " INFO_FORMAT "\n"\
			, errno, __FILE__, __LINE__, __func__,\
			(long) getpid(), (long) getppid(), (long) pthread_self());\
	ERR_STACK_TRACE();\
	errno = originalErrno;\
	perror(source);\
	cleanup\
	ERR_EXIT(ERR_MULTIPROCESS);\
} while(0)
// 700IQ setup for having macros with optional arguments
// without simply using __VAR_ARGS__ lol
// courtesy of stackoverflow.com/q/3046889
#define ERR_0_ARGS() ERR_(LBLUE"Error Description"RESET_ESC,)
#define ERR_1_ARG(source) ERR_(source,)
#define ERR_2_ARGS(source, cleanup) ERR_(source, cleanup)
#define ERR_X_ARGS(x, A, B, FUNC, ...) FUNC
#define ERR(...) ERR_X_ARGS(, ##__VA_ARGS__,\
	ERR_2_ARGS(__VA_ARGS__),\
	ERR_1_ARG(__VA_ARGS__),\
	ERR_0_ARGS(__VA_ARGS__))\

#if EXTENSIONS_ALLOWED

// kinda overkill? but I guess this version should let you use the
// comparison value afterwards
// should I remove int and go back to bool btw? xdd
#define USAGE(correctUsage) (__extension__({\
	int assertion = (int) (correctUsage);\
	if (!assertion) {\
		ERR_NEG(fprintf(stderr,\
					RED_BLD_INV"BAD ARGS" RESET_ESC": " LRED_BLD"!" RESET_ESC"( " LBLUE#correctUsage\
					RESET_ESC" )\n\n" LRED_BLD"Usage" RESET_ESC":\n" BWHITE"$ %s" RESET_ESC" %s\n"\
					, argv[0], usageDescription));\
		ERR_EXIT(ERR_MULTIPROCESS);\
	}\
	assertion;\
}))

#define ERR_IF(expr, condition, castType, ...) (__extension__({\
	errno = 0;\
	castType exprVal = (castType) (expr);\
	if ( condition exprVal )\
	{ERR(LRED_BLD#condition RESET_ESC" ( " LBLUE#expr RESET_ESC" ) " RESET_ESC, ##__VA_ARGS__);}\
	exprVal;\
}))
#define DBGprintf(format, ...) (__extension__({\
	int printfRet = ERR_NEG(printf(LRED_BLD_INV"DBG:" RESET_ESC" " INFO_FORMAT "\n" BWHITE format RESET_ESC\
				, __FILE__, __LINE__, __func__,\
				(long) getpid(), (long) getppid(), (long) pthread_self(), ##__VA_ARGS__));\
	ERR_EOF(fflush(stdout));\
	printfRet;\
}))

#else // EXTENSIONS_ALLOWED

#define USAGE(correctUsage) do {\
	if (!(correctUsage)) {\
		ERR_NEG(fprintf(stderr,\
					RED_BLD_INV"BAD ARGS" RESET_ESC": " LRED_BLD"!" RESET_ESC"( " LBLUE#correctUsage\
					RESET_ESC" )\n\n" LRED_BLD"Usage" RESET_ESC":\n" BWHITE"$ %s" RESET_ESC" %s\n"\
					, argv[0], usageDescription));\
		ERR_EXIT(ERR_MULTIPROCESS);\
	}\
} while(0)

// NB: castType is ignored here, because I'm too lazy lol
// not the best solution but whatever
// TODO: FIX THIS LOL
#define ERR_IF(expr, condition, castType, ...) do {\
	errno = 0;\
	castType exprVal = (castType) (expr);\
	if ( condition exprVal )\
	{ERR(LRED_BLD#condition RESET_ESC" ( " LBLUE#expr RESET_ESC" ) " RESET_ESC, ##__VA_ARGS__);}\
} while (0)
#define DBGprintf(format, ...) do {\
	ERR_NEG(printf(LRED_BLD_INV"DBG:" RESET_ESC" " INFO_FORMAT "\n" BWHITE format RESET_ESC\
				, __FILE__, __LINE__, __func__,\
				(long) getpid(), (long) getppid(), (long) pthread_self(), ##__VA_ARGS__));\
	ERR_EOF(fflush(stdout));\
} while(0)

#endif // EXTENSIONS_ALLOWED

// again for consistency
#define usage_(correctUsage) USAGE(correctUsage)

// version of DBGprintf that only compiles in DEBUG_MODE
// kinda want DBGprintf to be nice and accesible normally
#if DEBUG_MODE
#define DBGonly(...) DBGprintf(__VA_ARGS__)
#else // DEBUG_MODE
#define DBGonly(...)
#endif // DEBUG_MODE

#if EXTENSIONS_ALLOWED
#define myRetry(expr, castType) (__extension__({\
    castType val;\
    do {val = (castType) (expr);} \
    while(errno == EINTR);\
    val;\
}))
#else // EXTENSIONS_ALLOWED
// lol:
#define myRetry(expr, castType) TEMP_FAILURE_RETRY(expr)
#endif // EXTENSIONS_ALLOWED

#define ERR_NEG1(expr, ...) ERR_IF(expr, -1 ==, int, ##__VA_ARGS__)
#define ERR_NEG(expr, ...) ERR_IF(expr, 0 > , int, ##__VA_ARGS__)
#define ERR_NON_ZERO(expr, ...) ERR_IF(expr, 0 !=, int, ##__VA_ARGS__)
#define ERR_EOF(expr, ...) ERR_IF(expr, EOF ==, int, ##__VA_ARGS__)
#define ERR_NULL(expr, ...) ERR_IF(expr, NULL ==, void*, ##__VA_ARGS__)
#define ERR_SSIZE_T(expr, ...) ERR_IF(expr, -1 ==, ssize_t, ##__VA_ARGS__)
#define ERR_MQ(expr, ...) ERR_IF(expr, (mqd_t) -1 ==, mqd_t, ##__VA_ARGS__)
#define ERR_IN_ADDR_T(expr, ...) ERR_IF(expr, (in_addr_t) -1 ==, in_addr_t, ##__VA_ARGS__)
#define ERR_BARRIER(expr, ...) ERR_IF(expr, PTHREAD_BARRIER_SERIAL_THREAD != exprVal && 0 !=, int, ##__VA_ARGS__)
#define ERR_SEM(expr, ...) ERR_IF(expr, SEM_FAILED ==, sem_t, ##__VA_ARGS__)

// versions that allow a single okay errno value:
#define ERR_IF_(expr, condition, acceptableError, castType, ...) ERR_IF(expr, errno != acceptableError && condition, castType, ##__VA_ARGS__)
#define ERR_NEG1_(expr, acceptableError, ...) ERR_IF_(expr, -1 ==, acceptableError, int, ##__VA_ARGS__)
#define ERR_NEG_(expr, acceptableError, ...) ERR_IF_(expr, 0 >, acceptableError, int, ##__VA_ARGS__)
#define ERR_NON_ZERO_(expr, acceptableError, ...) ERR_IF_(expr, 0 !=, acceptableError, int, ##__VA_ARGS__)
#define ERR_EOF_(expr, acceptableError, ...) ERR_IF_(expr, EOF == , acceptableError, int, ##__VA_ARGS__)
#define ERR_NULL_(expr, acceptableError, ...) ERR_IF_(expr, NULL ==, acceptableError, void*, ##__VA_ARGS__)
#define ERR_SSIZE_T_(expr, acceptableError, ...) ERR_IF(expr, -1 ==, acceptableError, ssize_t, ##__VA_ARGS__)
#define ERR_MQ_(expr, acceptableError, ...) ERR_IF_(expr, (mqd_t) -1 ==, acceptableError, mqd_t, ##__VA_ARGS__)
#define ERR_IN_ADDR_T_(expr, acceptableError, ...) ERR_IF(expr, (in_addr_t) -1 ==, acceptableError, in_addr_t, ##__VA_ARGS__)
#define ERR_BARRIER_(expr, ...) ERR_IF(expr, PTHREAD_BARRIER_SERIAL_THREAD != exprVal && 0 !=, acceptableError, int, ##__VA_ARGS__)
#define ERR_SEM_(expr, acceptableError, ...) ERR_IF(expr, SEM_FAILED ==, acceptableError, sem_t, ##__VA_ARGS__)

// UGHHH YUCK GROSSS
// TODO: make these names better
#define CHECK_RETRY_BASE(expr, err, errType, ...) err( myRetry(expr, errType) , ##__VA_ARGS__)

#define CHECK_RETRY(expr, ...) CHECK_RETRY_BASE(expr, ERR_NEG1, int, ##__VA_ARGS__)
#define CHECK_RETRY_MQ(expr, ...) CHECK_RETRY_BASE(expr, ERR_MQ, mqd_t, ##__VA_ARGS__)
#define CHECK_RETRY_SSIZE_T(expr, ...) CHECK_RETRY_BASE(expr, ERR_SSIZE_T, ssize_t, ##__VA_ARGS__)
#define CHECK_RETRY_EOF(expr, ...) CHECK_RETRY_BASE(expr, ERR_EOF, int, ##__VA_ARGS__)

#define CHECK_RETRY_(expr, ...) CHECK_RETRY_BASE(expr, ERR_NEG1_, int, ##__VA_ARGS__)
#define CHECK_RETRY_MQ_(expr, ...) CHECK_RETRY_BASE(expr, ERR_MQ_, mqd_t, ##__VA_ARGS__)
#define CHECK_RETRY_SSIZE_T_(expr, ...) CHECK_RETRY_BASE(expr, ERR_SSIZE_T_, ssize_t, ##__VA_ARGS__)
#define CHECK_RETRY_EOF_(expr, ...) CHECK_RETRY_BASE(expr, ERR_EOF_, int, ##__VA_ARGS__)
// in theory there's more types to check but whoooooooo caaaaares

#endif // ERROR_HELPERS_H