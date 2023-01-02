#include "../stringCrap.h"

int main(int argc, char** argv) {
    char* usageDescription = "str";
    usage_(argc > 1);

    printf("%ld\n", charPos(argv[1], 'q'));
    printf("%s\n", (char*) leftShift_cycle(argv[1], strlen(argv[1]), 2));
    printf("%s\n", (char*) removeNinstances(argv[1], 'q', -1));

    printf("%s\n", (char*) rightShift_fill(argv[1], strlen(argv[1]), 3, 'x'));
    printf("%s\n", (char*) rightShift(argv[1], strlen(argv[1]), 3));
}