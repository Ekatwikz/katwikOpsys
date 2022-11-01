// side efect: a bunch of headers that it needs are already included :)
#include <sys/stat.h>
#include <fcntl.h>

#define ERR_MULTIPROCESS 0
//#define ATTRIBUTES_ALLOWED 0
#define USAGE_STRING "n filename\nn - amount to generate"
//#define USE_RAND_R 1
#include "katwikOpsys.h"

int main (int argc, char** argv) {
	char* nums;
	int n;
	int fileDescriptor;

	USAGE(argc == 3);
	USAGE( (n = atoi(argv[1])) >= 1 );
	ERR_NEG1(fileDescriptor = open(argv[2], O_WRONLY | O_APPEND));

	ERR_NULL(nums = malloc(n));

	//myRandSleep(1, 2, NULL);

	srand(time(NULL));
	for (int i = 0; i < n; ++i)
		nums[i] = myRand('0', '9');

    printBuf(nums, n); // no '\0'? easy :)

	ERR_NEG1(write(fileDescriptor, nums, n));
	ERR_NEG1(close(fileDescriptor));

	FREE(nums); // slightly safer, trust me xD

	ERR_NEG1(-1,
		printf("huhu\n");
		printf("hihi\n"););
}