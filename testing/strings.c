#include "../stringCrap.h"
//USAGE_STRING="str"

int main(int argc, char** argv) {
    USAGE(argc > 1);

    printf("%ld\n", charPos(argv[1], 'q'));
    printf("%s\n", (char*) leftShift_cycle(argv[1], strlen(argv[1]), 2));
    printf("%s\n", (char*) removeNinstances(argv[1], 'q', -1));
}