#include "katwikOpsys.h"

#define BUF_SIZE 128

int main(int argc, char** argv) {
	char* usageDescription = "inputFile outputFile chunks\nwhere chunks >= 1";

	usage_(argc == 4);
	char* inputFile = argv[1];
	char* outputFile = argv[2];

	int chunks;
	usage_(( chunks = atoi(argv[3]) ) >= 1);

	int inputFildes = open_(inputFile, O_RDONLY),
		outputFildes = open_(outputFile, O_WRONLY);

	char* readBuffer = malloc_(chunks * BUF_SIZE * sizeof(char));
	parallelIO(NULL, inputFildes, 2, BUF_SIZE, (void*) readBuffer, NULL, chunks, READ);

	printBuf(readBuffer, chunks * BUF_SIZE);
	free_(readBuffer);

	close_(inputFildes);
	close_(outputFildes);

	return EXIT_SUCCESS;
}
