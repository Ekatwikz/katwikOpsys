#include "errorHelpers.h"

// cutely prints output of "addr2line -f -e filename address" cmd to a stream
static void MY_NON_NULL(1, 2, 3)
	myAddr2LinePrinter(FILE* stream, char* restrict line1, char* restrict line2, int oflag) {
		if (oflag & TRUNCATE_RESERVED && *line1 == '_') {
			fprintf(stream, VIOLET_BLD"\t[" BWHITE" Reserved Symbol " VIOLET_BLD"]\n"RESET_ESC);
			return;
		} else if (oflag & TRUNCATE_UNKNOWN && *line1 == '?') {
			fprintf(stream, VIOLET_BLD"\t[" BWHITE" ?? " VIOLET_BLD"]\n"RESET_ESC);
			return;
		} else if (oflag & TRUNCATE_TRACE_FUNCTIONS && ( !strcmp(line1, "myStackTracer\n") || !strcmp(line1, "backtrace\n"))) {
			fprintf(stream, VIOLET_BLD"\t[" BWHITE" Stack Trace Function " VIOLET_BLD"]\n"RESET_ESC);
			return;
		}

		fprintf(stream, VIOLET_BLD"\t");
		// lmfao wtf?? print the line char by char just to ignore the '\n'??
		// lmao make this better
		for (char* curr = line1; *curr && *curr != '\n'; ++curr) {
			fputc(*curr, stream);
		}

		fprintf(stream, BWHITE" ( )\n" GREEN_BLD"\t");
		int lineNumberPrinted = 0;
		for (char* curr = line2; *curr; ++curr) {
			switch (*curr) {
				case ':':
					{
						fprintf(stream, RESET_ESC":" YELLLOW);
						lineNumberPrinted = 1; // i guess lol, usually works anyway
						// just uhh don't have ':' in paths tho? idk lol
						break;
					}
				case '(':
					{
						if (lineNumberPrinted) {
							// for printing discriminator thingy?
							fprintf(stream, RESET_ESC"(");
						} else goto justPutCharLol; // the usual -Wimplicit-fallthrough hax
						break;
					}
				default:
justPutCharLol:
					{
						fputc(*curr, stream);
						break;
					}
			}
		}

// shhhh ;)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-zero-length"
        fprintf(stream, RESET_ESC);
#pragma GCC diagnostic pop
		return;
	}

// NB: DON'T link with -rdynamic b/c of this function
// the internet tells lies lol
//
// also this function is pretty unsafe lol
// you can turn it off with TRACE_ON_ERR=0
#define MAX_TRACE 150 /* even super complicated programs don't need this much tbh lol */
void myStackTracer(FILE* stream,
		int oflag // passed to myAddr2LinePrinter
		) {
	void* traceBuffer[MAX_TRACE];
	int size = backtrace(traceBuffer, MAX_TRACE);
	char** traceStrings;
	fprintf(stream, RESET_ESC"Call stack trace");

	if (NULL == (traceStrings = backtrace_symbols(traceBuffer, size))) {
		fprintf(stream, LRED_BLD" [Couldn't backtrace_symbols( )]\n"RESET_ESC);
		FREE(traceStrings);
		return;
	}

	FILE* execToFile;
	int ranOutOfSpaceAt;
	EXEC_ONLY {
		// extra char for '\0'
		char execBuffer[MAX_EXEC_STR + 1] = {0};

		// create the execution string for addr2line
		char* s2Curr = execBuffer;

		char* commandStr = "addr2line -f -e ";
		size_t commandStrSz = strlen(commandStr);
		ranOutOfSpaceAt = size;

		// TODO: better variable names here
		// ALSO BRUH WTF WHY DIDN'T I JUST USE MEMCPY AAAAA
		for (int i = 0, fail = 0; i < size; ++i) {
			char* sCurr = traceStrings[i]; 
			//s2Curr += sprintf(s2Curr, "addr2line -f -e ");

			if (s2Curr > execBuffer + MAX_EXEC_STR - commandStrSz)  {
				ranOutOfSpaceAt = i;
				//printf("SPRINTF\n");
				break;
			} else {
				memcpy(s2Curr, commandStr, commandStrSz);
				s2Curr += commandStrSz;
			}

			for (int done = 0; *sCurr && !done; ++sCurr, ++s2Curr) {
				if (s2Curr > execBuffer + MAX_EXEC_STR - 1)  {
					ranOutOfSpaceAt = i;
					//printf("SINGLE\n");
					fail = 1;
					break;
				} else
					switch (*sCurr) {
						case ')':
							{
								done = 1;
								goto replaceBrace; // Wimplicit-fallthrough hax lol
							}
						case '(':
replaceBrace:
							{
								*s2Curr = ' ';
								break;   
							}
						default:
							{
								*s2Curr = *sCurr;
								break;
							}
					}
			}
			if (fail) {
				break;
			}

			if (s2Curr > execBuffer + MAX_EXEC_STR - 1)  {
				ranOutOfSpaceAt = i;
				printf("SINGLE2\n");
				break;
			} else {
				*s2Curr = ';';
				++s2Curr;
			}
		}

		//fprintf(stderr, "%s\n%d\n\n", execBuffer, ranOutOfSpaceAt);
		if (NULL == (execToFile = popen(execBuffer, "r"))) {
			fprintf(stream, LRED_BLD" [Couldn't popen( )]\n"RESET_ESC);
			FREE(traceStrings);
			return;
		}
	} else {
		UNUSED(execToFile); // necessary? idk lol
		UNUSED(ranOutOfSpaceAt);
	}

	fprintf(stream,
	", Top %d frames from top to bottom:\n", size);
	EXEC_ONLY {
		fprintf(stream, "A few line numbers could be iffy sometimes\n");
	}
	fprintf(stream, "+ god help you if this trace happens in multiple threads at once " BWHITE"\u2620" RESET_ESC"\n");

	char line1[MAX_EXEC_LINE + 1], line2[MAX_EXEC_LINE + 1];
	for (int i = 0, badTraceMessagePrinted = 0; i < size; ++i) {
		fprintf(stream, BWHITE"#%d\t" RESET_ESC" %s\n"RESET_ESC, i, traceStrings[i]);

		EXEC_ONLY {
			if (ranOutOfSpaceAt == i) {
				if (!badTraceMessagePrinted) {
					fprintf(stream, LRED_BLD
							"\t[ Ran out of stack space for buffer to pretty-fy stack trace ]\n"
							"\t[ Consider setting MAX_EXEC_STR to a higher value ]\n"RESET_ESC);
					badTraceMessagePrinted = 1;
				}
				continue;
			}

			// extra char for '\0'
			fgets(line1, MAX_EXEC_LINE, execToFile);
			fgets(line2, MAX_EXEC_LINE, execToFile);
			myAddr2LinePrinter(stream, line1, line2, oflag);
		}
	}

	EXEC_ONLY {
		if (pclose(execToFile) == -1) {
			fprintf(stream, LRED_BLD"[Couldn't pclose( )]\n"RESET_ESC);
			perror(LRED_BLD"[Couldn't pclose( )]"RESET_ESC);
		}
	}

	fflush(stream);
	FREE(traceStrings);
}