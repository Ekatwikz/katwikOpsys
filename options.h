#ifndef OPTIONS_H
#define OPTIONS_H

#ifndef HEADER_OKAY
#define HEADER_OKAY
#endif // HEADER_OKAY

// For less dumb header noise at compile time lul:
#ifndef MUNDANE_MESSAGES
#define MUNDANE_MESSAGES 1
#elif MUNDANE_MESSAGES == 1
#pragma message "Mundane messages have been manually turned on"
#elif MUNDANE_MESSAGES != 0
#pragma GCC error "MUNDANE_MESSAGES macro bad values.\nValid values are 0 (off) or 1 (on)"
#undef HEADER_OKAY
#endif // MUNDANE_MESSAGES

// 1 for some fun 4bit colors, 0 for boring
#ifndef ANSI_COLOR_FIESTA
#define ANSI_COLOR_FIESTA 1
#elif ANSI_COLOR_FIESTA == 1
#if MUNDANE_MESSAGES
#pragma message "Noice colorz have been manually turned on"
#endif // MUNDANE_MESSAGES
#elif ANSI_COLOR_FIESTA == 0
#pragma GCC warning "Bruhh no noice colorz?"
#else
#pragma GCC error "ANSI_COLOR_FIESTA macro bad value.\nValid values are 0 (off) or 1 (on)"
#undef HEADER_OKAY
#endif // ANSI_COLOR_FIESTA

// 1 to use rand_r in randomizers
#ifndef USE_RAND_R
#define USE_RAND_R 0
#elif USE_RAND_R == 1
#if MUNDANE_MESSAGES
#pragma message "Usage of rand_r has been manully turned on, you will have to pass a seed to all randomizers"
#endif // MUNDANE_MESSAGES
#elif USE_RAND_R == 0
#pragma message "Usage of rand_r has been manully turned off"
#else
#pragma GCC error "USE_RAND_R macro bad value.\nValid values are 0 (off) or 1 (on)"
#undef HEADER_OKAY
#endif // USE_RAND_R

// 1 for moar info, 0 normally
#ifndef DEBUG_MODE
#define DEBUG_MODE 0
#elif DEBUG_MODE == 1
#if MUNDANE_MESSAGES
#pragma message "Debug mode has been manually turned on"
#endif // MUNDANE_MESSAGES
#elif DEBUG_MODE == 0
#if MUNDANE_MESSAGES
#pragma message "Debug mode has been manually turned off"
#endif // MUNDANE_MESSAGES
#elif DEBUG_MODE == 0
#else
#pragma GCC error "DEBUG_MODE macro bad value.\nValid values are 0 (off) or 1 (on)"
#undef HEADER_OKAY
#endif // DEBUG_MODE

// seems dumb but might want to also have the option to disable these
// even when __GNUC__ is defined 
#ifndef ATTRIBUTES_ALLOWED
#define ATTRIBUTES_ALLOWED 1
#elif ATTRIBUTES_ALLOWED == 1
#if MUNDANE_MESSAGES
#pragma message "Attributes have been manually allowed"
#endif // MUNDANE_MESSAGES
#elif ATTRIBUTES_ALLOWED == 0
#if MUNDANE_MESSAGES
#pragma message "Attributes have been manually disallowed"
#endif // MUNDANE_MESSAGES
#else
#pragma GCC error "ATTRIBUTES_ALLOWED macro bad value.\nValid values are 1 (allowed) or 0 (disallowed)"
#undef HEADER_OKAY
#endif // ATTRIBUTES_ALLOWED

// config for whether or not some int-error functions from this header should just be void,
// immediately using ERR instead of returning an error code
#ifndef RETURN_ERR
#define RETURN_ERR 0
#elif RETURN_ERR == 1
#if MUNDANE_MESSAGES
#pragma message "Returning error codes from this header has been manually allowed"
#endif // MUNDANE_MESSAGES
#elif RETURN_ERR == 0
#if MUNDANE_MESSAGES
#pragma message "Returning error codes from this header has been manually disallowed"
#endif // MUNDANE_MESSAGES
#else
#pragma GCC error "RETURN_ERR macro bad value.\nValid values are 1 (allowed) or 0 (disallowed)"
#undef HEADER_OKAY
#endif // RETURN_ERR

// mainly for braced groups in the seksi macros lol
#ifndef EXTENSIONS_ALLOWED
#define EXTENSIONS_ALLOWED 1
#elif EXTENSIONS_ALLOWED == 1
#if MUNDANE_MESSAGES
#pragma message "header's usage of __extension__ has been manually allowed"
#endif // MUNDANE_MESSAGES
#elif EXTENSIONS_ALLOWED == 0
#if MUNDANE_MESSAGES
#pragma message "header's usage of __extension__ has been manually disallowed"
#endif // MUNDANE_MESSAGES
#else
#pragma GCC error "EXTENSIONS_ALLOWED macro bad value.\nValid values are 1 (allowed) or 0 (disallowed)"
#undef HEADER_OKAY
#endif // EXTENSIONS_ALLOWED

#ifndef TRACE_ON_ERR
#define TRACE_ON_ERR 1
#elif TRACE_ON_ERR == 1
#if MUNDANE_MESSAGES
#pragma message "Tracing when ERR happens has been manually enabled"
#endif // MUNDANE_MESSAGES
#elif TRACE_ON_ERR == 0
#if MUNDANE_MESSAGES
#pragma message "Tracing when ERR happens has been manually disallowed"
#endif // MUNDANE_MESSAGES
#else
#pragma GCC error "TRACE_ON_ERR macro bad value.\nValid values are 1 (allowed) or 0 (disallowed)"
#undef HEADER_OKAY
#endif // TRACE_ON_ERR

#ifndef EXEC_ALLOWED
#define EXEC_ALLOWED 1
#elif EXEC_ALLOWED == 1
#if MUNDANE_MESSAGES
#pragma message "Header's usage of exec (and similar functions) has been manually allowed"
#endif // MUNDANE_MESSAGES
#elif EXEC_ALLOWED == 0
#if MUNDANE_MESSAGES
#pragma message "Header's usage of exec (and similar functions) has been manually disallowed"
#endif // MUNDANE_MESSAGES
#else
#pragma GCC error "EXEC_ALLOWED macro bad value.\nValid values are 1 (allowed) or 0 (disallowed)"
#undef HEADER_OKAY
#endif // EXEC_ALLOWED

#endif // OPTIONS_H