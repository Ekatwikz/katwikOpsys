#define ERR_MULTIPROCESS 0
#define USAGE_STRING "bruh"

//#define EXEC_ALLOWED 0
//#define ATTRIBUTES_ALLOWED 1
//#define ANSI_COLOR_FIESTA 0
//#define EXEC_ALLOWED 0

//#include "../katwikOpsys.h"
#include "../easyCheck.h"
//#include "../errorHelpers.h"

int errsNeg1(int shouldErr) {
    if (shouldErr == -1) {
        errno = EPERM;
    }

    return shouldErr;
}

int main (int argc, char** argv) {
    close_(STDIN_FILENO);
    close_(69);
     
    USAGE(argc > 1);
    printf_("%s\n", argv[1]);
    ERR_NEG1(errsNeg1(atoi(argv[1])));
}