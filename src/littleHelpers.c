#include "littleHelpers.h"

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

void MY_NON_NULL(1, 2)
	swap_(void* restrict lhs, void* restrict rhs, size_t sz, void* restrict buffer) {
		void* tmp;
		if (buffer) {
			// having this option might be useful for frequent swaps
			tmp = buffer;
		} else {
			// but alloc a buffer if aren't given one
			ERR_NULL(tmp = malloc(sz));
		}

		memcpy(tmp, lhs, sz);
		memcpy(lhs, rhs, sz);
		memcpy(rhs, tmp, sz);

		if (!buffer) {
			FREE(tmp);
		}
	}
