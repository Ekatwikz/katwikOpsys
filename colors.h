// yey colorz :)
#ifndef MY_COLORS_H
#define MY_COLORS_H

#include "options.h"

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
#endif // ANSI_COLOR_FIESTA

#if ANSI_COLOR_FIESTA
#define RESET_ESC			"\033[0m"
#define DBLUE RESET_ESC			"\033[34m"
#define RED_BLD_INV RESET_ESC		"\033[31;1;7m"
#define LBLUE RESET_ESC			"\033[96m"
#define LRED_BLD RESET_ESC		"\033[91;1m"
#define LRED_BLD_INV RESET_ESC		"\033[91;1;7m"
#define LBLUE_BLD_INV RESET_ESC		"\033[94;1;7m"
#define VIOLET_BLD RESET_ESC		"\033[35;1m"
#define BWHITE RESET_ESC 		"\033[97m"
#define BWHITE_INV RESET_ESC		"\033[97;7m"
#define GREEN_BLD RESET_ESC		"\033[32;1m"
#define YELLLOW RESET_ESC 		"\033[33m"

#else
#define RESET_ESC			""
#define DBLUE				""
#define RED_BLD_INV			""
#define LBLUE				""
#define LRED_BLD			""
#define LRED_BLD_INV			""
#define LBLUE_INV RESET_ESC		""
#define VIOLET_BLD			""
#define BWHITE				""
#define BWHITE_INV			""
#define GREEN_BLD			""
#define YELLLOW				""
#endif // ANSI_COLOR_FIESTA

#endif // MY_COLORS_H
