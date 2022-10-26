// yey colorz :)
#ifndef MYCOLORS_H
#define MYCOLORS_H

#include "options.h"

#if ANSI_COLOR_FIESTA
#define RESET_ESC		"\033[0m"
#define DBLUE RESET_ESC		"\033[34m"
#define RED_BLD_INV RESET_ESC	"\033[31;1;7m"
#define LBLUE RESET_ESC		"\033[96m"
#define LRED_BLD RESET_ESC	"\033[91;1m"
#define LRED_BLD_INV RESET_ESC	"\033[91;1;7m"
#define VIOLET_BLD RESET_ESC	"\033[35;1m"
#define BWHITE RESET_ESC 	"\033[97m"
#define BWHITE_INV RESET_ESC	"\033[97;7m"
#define GREEN_BLD RESET_ESC	"\033[32;1m"
#define YELLLOW RESET_ESC 	"\033[33m"

#else
#define RESET_ESC		""
#define DBLUE			""
#define RED_BLD_INV		""
#define LBLUE			""
#define LRED_BLD		""
#define LRED_BLD_INV		""
#define VIOLET_BLD		""
#define BWHITE			""
#define BWHITE_INV		""
#define GREEN_BLD		""
#define YELLLOW			""
#endif // ANSI_COLOR_FIESTA

#endif // MYCOLORS_H