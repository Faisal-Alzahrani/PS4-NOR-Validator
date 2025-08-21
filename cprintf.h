/* console.h -- console C89 header file
 *
 * Copyright (C) 2016 Christian Winkler
 * Copyright (C) 2023 Faisal Alzahrani
 * 
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

//The origial code needed some formatting..

#ifndef CPRINT_H
#define CPRINT_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CC_RESET   "\x1b[0m"
#define CF_BLACK   "\x1b[30m"
#define CF_MAROON  "\x1b[31m"
#define CF_GREEN   "\x1b[32m"
#define CF_OLIVE   "\x1b[33m"
#define CF_NAVY    "\x1b[34m"
#define CF_PURPLE  "\x1b[35m"
#define CF_TEAL    "\x1b[36m"
#define CF_SILVER  "\x1b[37m"
#define CF_GREY    "\x1b[90m"
#define CF_RED     "\x1b[91m"
#define CF_LIME    "\x1b[92m"
#define CF_YELLOW  "\x1b[93m"
#define CF_BLUE    "\x1b[94m"
#define CF_PINK    "\x1b[95m"
#define CF_AQUA    "\x1b[96m"
#define CF_WHITE   "\x1b[97m"

#define CB_BLACK   "\x1b[40m"
#define CB_MAROON  "\x1b[41m"
#define CB_GREEN   "\x1b[42m"
#define CB_OLIVE   "\x1b[43m"
#define CB_NAVY    "\x1b[44m"
#define CB_PURPLE  "\x1b[45m"
#define CB_TEAL    "\x1b[46m"
#define CB_SILVER  "\x1b[47m"
#define CB_GREY    "\x1b[100m"
#define CB_RED     "\x1b[101m"
#define CB_LIME    "\x1b[102m"
#define CB_YELLOW  "\x1b[103m"
#define CB_BLUE    "\x1b[104m"
#define CB_PINK    "\x1b[105m"
#define CB_AQUA    "\x1b[106m"
#define CB_WHITE   "\x1b[107m"


typedef enum 
{
    BLACK  = 30,
    MAROON = 31,
    GREEN  = 32,
    OLIVE  = 33,
    NAVY   = 34,
    PURPLE = 35,
    TEAL   = 36,
    SILVER = 37,
    GREY   = 90,
    RED    = 91,
    LIME   = 92,
    YELLOW = 93,
    BLUE   = 94,
    PINK   = 95,
    AQUA   = 96,
    WHITE  = 97 
} Color;

/* clear console content */
void cc_clear(void);
/* reset foreground and background color */
void cc_reset(void);
/* set foreground color */
void cc_set_color(Color color);
/* set background color */
void cc_set_background_color(Color color);
/* set cursor position */
void cc_set_curpos(int x, int y);

//added this function for convenience..
void cprintf(FILE* out, const char* format, ...);

#ifdef __cplusplus
}
#endif

#endif /* CPRINT_H */
