#include "katwikOpsys.h"
//USAGE_STRING="inputFile outputFile chunks\nwhere chunks >= 1"

#include <sys/types.h>

#define BUF_SIZE 128

int main(int argc, char** argv) {
	USAGE(argc == 4);
	char* inputFile = argv[1];
	char* outputFile = argv[2];

	int chunks;
	USAGE(( chunks = atoi(argv[3]) ) >= 1);

	int inputFildes = open_(inputFile, O_RDONLY),
		outputFildes = open_(outputFile, O_WRONLY);

	char* readBuffer = malloc_(chunks * BUF_SIZE * sizeof(char));
	parallelIO(NULL, inputFildes, 2, BUF_SIZE, (void*) readBuffer, NULL, chunks, READ);

	printBuf(readBuffer, chunks * BUF_SIZE);
	FREE(readBuffer);

	close_(inputFildes);
	close_(outputFildes);

	return EXIT_SUCCESS;
}
