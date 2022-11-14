#include "katwikOpsys.h"

int main (int argc, char** argv) {
	char* usageDescription = "n filename\n"
							"n - amount of rands to generate\n"
							"filename - file to write";
	USAGE(argc == 3);

	int n = atoi(argv[1]);
	USAGE( n >= 1 );

	char* nums = malloc_(n);
	int fileDescriptor = open_(argv[2], O_WRONLY | O_APPEND);

	myRandSleep(1, 2);

	srand(time(NULL));
	for (int i = 0; i < n; ++i) {
		nums[i] = myRand('0', '9');
	}

    printBuf(nums, n); // no '\0'? easy :)

	write_(fileDescriptor, nums, n);
	close_(fileDescriptor);

	FREE(nums); // slightly safer
}