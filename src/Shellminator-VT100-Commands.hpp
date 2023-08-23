/*
 * Created on May 14 2023
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified 2022.04.16
*/

/*
MIT License

Copyright (c) 2023 Daniel Hajnal

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#ifndef SHELLMINATOR_VT100_HPP_
#define SHELLMINATOR_VT100_HPP_

#define VT100_CMD_CLEAR_TO_END  "\033[2K"
#define VT100_CMD_ARROW_UP      "\033[A"
#define VT100_CMD_ARROW_DOWN    "\033[B"
#define VT100_CMD_ARROW_LEFT    "\033[D"
#define VT100_CMD_ARROW_RIGHT   "\033[C"

// Format commands
#define VT100_FMT_REGULAR       "\033[0m"
#define VT100_FMT_BOLD          "\033[1m"
#define VT100_FMT_LOW_INTENSITY "\033[2m"
#define VT100_FMT_ITALIC        "\033[3m"
#define VT100_FMT_UNDERLINE     "\033[4m"
#define VT100_FMT_BLINKING      "\033[5m"
#define VT100_FMT_REVERSE       "\033[6m"
#define VT100_FMT_BACKGROUND    "\033[7m"
#define VT100_FMT_INVISIBLE     "\033[8m"

// Color commands
#define VT100_FMT_BLACK     "\033[30m"
#define VT100_FMT_RED       "\033[31m"
#define VT100_FMT_GREEN     "\033[32m"
#define VT100_FMT_YELLOW    "\033[33m"
#define VT100_FMT_BLUE      "\033[34m"
#define VT100_FMT_MAGENTA   "\033[35m"
#define VT100_FMT_CYAN      "\033[36m"
#define VT100_FMT_WHITE     "\033[37m"

#endif