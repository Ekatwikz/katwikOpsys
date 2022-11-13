#include <sys/stat.h>
#include <fcntl.h>

//USAGE_STRING="filename\nn - amount to generate"
#include "katwikOpsys.h"

int main (int argc, char** argv) {
	USAGE(argc == 3);

	int n = atoi(argv[1]);
	USAGE( n >= 1 );

	char* numms = malloc_(n);
	int fileDescriptor = open_(argv[2], O_WRONLY | O_APPEND);

	myRandSleep(1, 2, NULL);

	srand(time(NULL));
	for (int i = 0; i < n; ++i)
		nums[i] = myRand('0', '9');

    printBuf(nums, n); // no '\0'? easy :)

	write_(fileDescriptor, nums, n);
	close_(fileDescriptor);

	FREE(nums); // slightly safer
}