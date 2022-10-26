#define ERR_MULTIPROCESS 0
#define USAGE_STRING "bruh"
#include "../katwikOpsys.h"

int boonk(int shouldErr) {
    errno = shouldErr ? EPERM : 0;
    return shouldErr;
}

int main (int argc, char** argv) {
    USAGE(argc > 1);
    printf_("%s\n", argv[1]);

    ERR_NEG1(boonk(atoi(argv[1])));
}