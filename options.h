#ifndef OPTIONS_H
#define OPTIONS_H

// For less dumb header noise at compile time lul:
#ifndef MUNDANE_MESSAGES
#define MUNDANE_MESSAGES 1
#elif MUNDANE_MESSAGES == 1
#pragma message "Mundane messages have been manually turned on"
#elif MUNDANE_MESSAGES != 0
#pragma GCC error "MUNDANE_MESSAGES macro bad values.\nValid values are 0 (off) or 1 (on)"
#undef HEADER_OKAY
#endif // MUNDANE_MESSAGES

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

#endif // OPTIONS_H