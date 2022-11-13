#include "../katwikOpsys.h"
//USAGE_STRING="val"

int errsNeg1(int shouldErr) {
    if (shouldErr == -1) {
        errno = EPERM;
    }

    return shouldErr;
}

int main (int argc, char** argv) {
    close_(STDIN_FILENO);
    //close_(69);
    sem_t test = sem_make();
    UNUSED(test);

    USAGE(argc > 1);
    int val = atoi(argv[1]);

    //printf_("%s\n", argv[1]);
    ERR_NEG1(errsNeg1(val));
    return 0;
}