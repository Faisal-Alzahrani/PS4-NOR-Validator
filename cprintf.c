/* cpritf.c -- color enabled console module
 *
 * Copyright (C) 2016 Christian Winkler
 * Copyright (C) 2023 Faisal Alzahrani
 * 
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include <stdio.h>
#include <stdarg.h>
#include "cprintf.h"

#if defined(_WIN32)
#include <io.h>
#include <windows.h>

static WORD win_default_attributes(HANDLE hConsole) 
{
	static WORD defaultAttributes = 0;
	CONSOLE_SCREEN_BUFFER_INFO info;

    if (!defaultAttributes && GetConsoleScreenBufferInfo(hConsole, &info)) 
    {
		defaultAttributes = info.wAttributes;
	}

	return defaultAttributes;
}

static void cc_win_set_attributes(int fg, int bg) 
{
    WORD defaultAttributes;
    CONSOLE_SCREEN_BUFFER_INFO info;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    defaultAttributes = win_default_attributes(hConsole);

    if (fg == -1 && bg == -1) 
    {
        SetConsoleTextAttribute(hConsole, defaultAttributes);
        return;
    }

    if (!GetConsoleScreenBufferInfo(hConsole, &info))
    {
        return;
    }

    if (fg != -1) 
    {
        info.wAttributes &= ~(info.wAttributes & 0x0F);
        info.wAttributes |= (WORD)fg;
    }

    if (bg != -1) 
    {
        info.wAttributes &= ~(info.wAttributes & 0xF0);
        info.wAttributes |= (WORD)bg;
    }

    SetConsoleTextAttribute(hConsole, info.wAttributes);
}

static void win_set_cursor_position(int x, int y) 
{
	COORD pos;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(hConsole, pos);
}

static void cc_win_clear_console() 
{
    //WORD defaultAttributes;
	COORD topLeft = { 0, 0 };
    DWORD written;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;

    //defaultAttributes = win_default_attributes(hConsole);

    if (!GetConsoleScreenBufferInfo(hConsole, &info)) {
        return;
    }

    FillConsoleOutputCharacterA(hConsole, ' ', info.dwSize.X * info.dwSize.Y, topLeft, &written);
    FillConsoleOutputAttribute(hConsole, info.wAttributes, info.dwSize.X * info.dwSize.Y, topLeft, &written);
    win_set_cursor_position(topLeft.X, topLeft.Y);
}

#elif defined(__APPLE__) || defined(__unix__) || defined(__unix)
#include <unistd.h>
#include <string.h>
#else
#error unsupported platform
#endif

static int cc_is_terminal(FILE *out) 
{
    return _isatty(_fileno(out)) != 0;
}

#if defined(_WIN64)|| defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
void cc_clear() 
{
    if (cc_is_terminal(stdout))
    {
        cc_win_clear_console();
    }
}

void cc_reset() {
    if (cc_is_terminal(stdout)) 
    {
        cc_win_set_attributes(-1, -1);
    }
}

void cc_set_color(Color color) 
{
    if (cc_is_terminal(stdout)) 
    {
        switch (color) 
        {
        case BLACK:
            cc_win_set_attributes(0, -1);
            break;
        case MAROON:
            cc_win_set_attributes(FOREGROUND_RED, -1);
            break;
        case GREEN:
            cc_win_set_attributes(FOREGROUND_GREEN, -1);
            break;
        case OLIVE:
            cc_win_set_attributes(FOREGROUND_GREEN | FOREGROUND_RED, -1);
            break;
        case NAVY:
            cc_win_set_attributes(FOREGROUND_BLUE, -1);
            break;
        case PURPLE:
            cc_win_set_attributes(FOREGROUND_BLUE | FOREGROUND_RED, -1);
            break;
        case TEAL:
            cc_win_set_attributes(FOREGROUND_BLUE | FOREGROUND_GREEN, -1);
            break;
        case SILVER:
            cc_win_set_attributes(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, -1);
            break;
        case GREY:
            cc_win_set_attributes(FOREGROUND_INTENSITY, -1);
            break;
        case RED:
            cc_win_set_attributes(FOREGROUND_RED | FOREGROUND_INTENSITY, -1);
            break;
        case LIME:
            cc_win_set_attributes(FOREGROUND_GREEN | FOREGROUND_INTENSITY, -1);
            break;
        case YELLOW:
            cc_win_set_attributes(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY, -1);
            break;
        case BLUE:
            cc_win_set_attributes(FOREGROUND_BLUE | FOREGROUND_INTENSITY, -1);
            break;
        case PINK:
            cc_win_set_attributes(FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY, -1);
            break;
        case AQUA:
            cc_win_set_attributes(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY, -1);
            break;
        case WHITE:
            cc_win_set_attributes(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY, -1);
            break;
        }
    }
}


void cc_set_background_color(Color color) 
{
    if (cc_is_terminal(stdout)) 
    {
        switch (color) 
        {
        case BLACK:
            cc_win_set_attributes(-1, 0);
            break;
        case MAROON:
            cc_win_set_attributes(-1, BACKGROUND_RED);
            break;
        case GREEN:
            cc_win_set_attributes(-1, BACKGROUND_GREEN);
            break;
        case OLIVE:
            cc_win_set_attributes(-1, BACKGROUND_GREEN | BACKGROUND_RED);
            break;
        case NAVY:
            cc_win_set_attributes(-1, BACKGROUND_BLUE);
            break;
        case PURPLE:
            cc_win_set_attributes(-1, BACKGROUND_BLUE | BACKGROUND_RED);
            break;
        case TEAL:
            cc_win_set_attributes(-1, BACKGROUND_BLUE | BACKGROUND_GREEN);
            break;
        case SILVER:
            cc_win_set_attributes(-1, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
            break;
        case GREY:
            cc_win_set_attributes(-1, BACKGROUND_INTENSITY);
            break;
        case RED:
            cc_win_set_attributes(-1, BACKGROUND_RED | BACKGROUND_INTENSITY);
            break;
        case LIME:
            cc_win_set_attributes(-1, BACKGROUND_GREEN | BACKGROUND_INTENSITY);
            break;
        case YELLOW:
            cc_win_set_attributes(-1, BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
            break;
        case BLUE:
            cc_win_set_attributes(-1, BACKGROUND_BLUE | BACKGROUND_INTENSITY);
            break;
        case PINK:
            cc_win_set_attributes(-1, BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY);
            break;
        case AQUA:
            cc_win_set_attributes(-1, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
            break;
        case WHITE:
            cc_win_set_attributes(-1, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
            break;
        }
    }
}

void cc_set_curpos(int x, int y) 
{
    if (cc_is_terminal(stdout)) 
    {
        win_set_cursor_position(x, y);
    }
}

//defined(__APPLE__) || defined(__unix__) || defined(__unix)
#else

void cc_clear() 
{
    if (cc_is_terminal(stdout))
    {
       fprintf(stdout, "\033[2J\033[;H");
    }
}

void cc_reset()
{
    if (cc_is_terminal(stdout)) 
    {
        fprintf(stdout, "\033[00m");
    }
}

void cc_set_color(Color color) 
{
    if (cc_is_terminal(stdout)) 
    {
        switch (color) {
        case BLACK:
            fprintf(stdout, "%s", "\033[30m");
            break;
        case MAROON:
            fprintf(stdout, "%s", "\033[31m");
            break;
        case GREEN:
            fprintf(stdout, "%s", "\033[32m");
            break;
        case OLIVE:
            fprintf(stdout, "%s", "\033[33m");
            break;
        case NAVY:
            fprintf(stdout, "%s", "\033[34m");
            break;
        case PURPLE:
            fprintf(stdout, "%s", "\033[35m");
            break;
        case TEAL:
            fprintf(stdout, "%s", "\033[36m");
            break;
        case SILVER:
            fprintf(stdout, "%s", "\033[37m");
            break;
        case GREY:
            fprintf(stdout, "%s", "\033[90m");
            break;
        case RED:
            fprintf(stdout, "%s", "\033[91m");
            break;
        case LIME:
            fprintf(stdout, "%s", "\033[92m");
            break;
        case YELLOW:
            fprintf(stdout, "%s", "\033[93m");
            break;
        case BLUE:
            fprintf(stdout, "%s", "\033[94m");
            break;
        case PINK:
            fprintf(stdout, "%s", "\033[95m");
            break;
        case AQUA:
            fprintf(stdout, "%s", "\033[96m");
            break;
        case WHITE:
            fprintf(stdout, "%s", "\033[97m");
            break;
        }
    }
}


void cc_set_background_color(Color color) 
{
    if (cc_is_terminal(stdout)) 
    {
        switch (color) 
        {
        case BLACK:
            fprintf(stdout, "%s", "\033[40m");
            break;
        case MAROON:
            fprintf(stdout, "%s", "\033[41m");
            break;
        case GREEN:
            fprintf(stdout, "%s", "\033[42m");
            break;
        case OLIVE:
            fprintf(stdout, "%s", "\033[43m");
            break;
        case NAVY:
            fprintf(stdout, "%s", "\033[44m");
            break;
        case PURPLE:
            fprintf(stdout, "%s", "\033[45m");
            break;
        case TEAL:
            fprintf(stdout, "%s", "\033[46m");
            break;
        case SILVER:
            fprintf(stdout, "%s", "\033[47m");
            break;
        case GREY:
            fprintf(stdout, "%s", "\033[100m");
            break;
        case RED:
            fprintf(stdout, "%s", "\033[101m");
            break;
        case LIME:
            fprintf(stdout, "%s", "\033[102m");
            break;
        case YELLOW:
            fprintf(stdout, "%s", "\033[103m");
            break;
        case BLUE:
            fprintf(stdout, "%s", "\033[104m");
            break;
        case PINK:
            fprintf(stdout, "%s", "\033[105m");
            break;
        case AQUA:
            fprintf(stdout, "%s", "\033[106m");
            break;
        case WHITE:
            fprintf(stdout, "%s", "\033[107m");
            break;
        }
    }
}

void cc_set_curpos(int x, int y) 
{
    if (cc_is_terminal(stdout)) 
    {
        fprintf(stdout, "\033[%d;%dH", y + 1, x + 1);
    }
}

#endif


#define BUFFER_SIZE 2048
static char buffer[BUFFER_SIZE];

void cprintf(FILE* out, const char* format, ...)
{
    va_list argp;
    va_start(argp, format);
    vsnprintf(buffer, BUFFER_SIZE, format, argp);
    va_end(argp);
    char* s = buffer;
    char* sc;
    int enableColor = cc_is_terminal(out);

    while ((sc = strstr(s, "\033[")))
    {
        int x = sc - s;

        if (x > 0)
        {
            char c = s[x];
            s[x] = '\0';
            fprintf(out, s);
            s[x] = c;
        }


        int len = 0;

        if (sc[3] == 'm')
        {
            len = 4;
        }
        else if (sc[4] == 'm')
        {
            len = 5;
        }
        else if (sc[5] == 'm')
        {
            len = 6;
        }
        else
        {
            len = x;
        }

        s = sc + len;

        if (len > 1 && enableColor)
        {
            if (len == 4 && sc[2] == '0')
            {
                cc_reset();
            }
            else
            {
                Color color = 0;

                if (len == 5)
                {
                    color = (sc[2] - '0') * 10 + sc[3] - '0';
                }
                else if (len == 6)
                {
                    color = (sc[2] - '0') * 100 + (sc[3] - '0') * 10 + sc[4] - '0';
                }

                if ((color >= 30 && color <= 37) || (color >= 90 && color <= 97))
                {
                    cc_set_color(color);
                }
                else if ((color >= 40 && color <= 47) || (color >= 100 && color <= 107))
                {
                    cc_set_background_color(color - 10);
                }
            }
        }

    }

    fprintf(out, s);
}