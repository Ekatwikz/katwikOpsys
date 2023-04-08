#include "../gpiodHelpers.h"

// just using this to see if I can get to link stage without errors
// linking+running this test ain't so ez lol
int main(int argc, char** argv) {
    UNUSED(argc);
    UNUSED(argv);

    gpiod_chip_open_by_number_(0);
}