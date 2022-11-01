// NB: you can have multiple lines in your USAGE_STRING
#define USAGE_STRING "inputFile outputFile chunks\nwhere chunks >= 1"
#define ERR_MULTIPROCESS 0
#include "katwikOpsys.h"

#include <sys/types.h>

#define BUF_SIZE 128

int main(int argc, char** argv) {
	USAGE(argc == 4);
	char* inputFile = argv[1];
	char* outputFile = argv[2];
	int chunks;
	USAGE(( chunks = atoi(argv[3]) ) >= 1);

	int inputFildes, outputFildes;
	ERR_NEG1(inputFildes = open(inputFile, O_RDONLY));
	ERR_NEG1(outputFildes = open(outputFile, O_WRONLY));

	char* readBuffer;
	ERR_NULL(readBuffer = malloc(chunks * BUF_SIZE * sizeof(char)));
	parallelIO(NULL, inputFildes, 2, BUF_SIZE, (void*) readBuffer, NULL, chunks, READ);

	printBuf(readBuffer, chunks * BUF_SIZE);
	FREE(readBuffer);

	ERR_NEG1(close(inputFildes));
	ERR_NEG1(close(outputFildes));

	return EXIT_SUCCESS;
}
