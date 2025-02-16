/*
 * Created on July 07 2024
 *
 * Copyright (c) 2020 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified July 07 2024
*/

/*
MIT License

Copyright (c) 2020 Daniel Hajnal

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

#ifndef SHELLMINATOR_NEOFETCH_HPP_
#define SHELLMINATOR_NEOFETCH_HPP_

#include "Shellminator.hpp"
#include "Shellminator-Helpers.hpp"

// What a beautiful piece of art
#define SHELLMINATOR_DEFAULT_NEOFETCH_ART   "\033[1;31m"                                               \
                                            "    ##     ##     ##     ##     ##     ##     ##\r\n"     \
                                            "   ####   ####   ####   ####   ####   ####   ####\r\n"    \
                                            "  ################################################\r\n"   \
                                            " ##################################################\r\n"  \
                                            " ##################################################\r\n"  \
                                            " #######*++=----------------------------=++*#######\r\n"  \
                                            " ####-.                                      .-####\r\n"  \
                                            " ###:                                          :###\r\n"  \
                                            " ##*                                            *##\r\n"  \
                                            " ##*                                            *##\r\n"  \
                                            " ##*                                            *##\r\n"  \
                                            " ##*\033[1;36m    .............::.     :::.............   \033[1;31m*##\r\n"  \
                                            " ##*\033[1;36m  =%:/=======>-%%%%%#=-=#%%%%%%%%%%%%%%%%%: \033[1;31m*##\033[1;36m\r\n"  \
                                            " ###==#-||++++++++*%%%%%*   #%%%%%%%\033[1;31m###\033[1;36m%%%%%%%*=###\r\n"  \
                                            " =+#**#=\\/%%%%%%%%%%%%%%-   =%%%%%\033[1;31m+.-***\033[1;36m%%%%%%#*#+=\033[1;33m\r\n"  \
                                            "   .  \033[1;36m+%%%%%%%%%%%%%%%%#\033[1;33m.....\033[1;36m#%%%%\033[1;31m+  ::=#\033[1;36m%%%%%=\033[1;33m .\r\n"    \
                                            ":==-  \033[1;36m+%%%%%%%%%%%%%%%%=\033[1;33m.....\033[1;36m#%%%%%\033[1;31m#***#\033[1;36m%%%%%%=\033[1;33m -==:\r\n" \
                                            "===-  \033[1;36m-%%%%%%%%%%%%%%%#\033[1;33m ......\033[1;36m%%%%%%%%%%%%%%%%:\033[1;33m -===\r\n" \
                                            "===-   \033[1;36m:-------------:\033[1;33m  .....  \033[1;36m:-------------.\033[1;33m  -===\r\n" \
                                            " -=-                    .....                   -=-\r\n"  \
                                            "                        .....\r\n"                        \
                                            "\r\n"

#define SHELLMINATOR_DEFAULT_NEOFETCH_ART_HEIGHT    22
#define SHELLMINATOR_DEFAULT_NEOFETCH_ART_WIDTH    54

void defaultShellminatorNeofetch( Shellminator* parent );

extern const char*  SHELLMINATOR_WEAK neofetch_header_text;
extern int          SHELLMINATOR_WEAK neofetch_header_text_color;
extern int          SHELLMINATOR_WEAK neofetch_header_text_style;

extern char         SHELLMINATOR_WEAK neofetch_header_line_char;
extern int          SHELLMINATOR_WEAK neofetch_header_line_color;
extern int          SHELLMINATOR_WEAK neofetch_header_line_style;

extern int          SHELLMINATOR_WEAK neofetch_field_text_color;
extern int          SHELLMINATOR_WEAK neofetch_field_text_style;

extern const char*  SHELLMINATOR_WEAK neofetch_fw_text;
extern int          SHELLMINATOR_WEAK neofetch_fw_text_color;
extern int          SHELLMINATOR_WEAK neofetch_fw_text_style;

extern const char*  SHELLMINATOR_WEAK neofetch_cpu_text;
extern int          SHELLMINATOR_WEAK neofetch_cpu_text_color;
extern int          SHELLMINATOR_WEAK neofetch_cpu_text_style;

extern const char*  SHELLMINATOR_WEAK neofetch_cpu_text;
extern int          SHELLMINATOR_WEAK neofetch_cpu_text_color;
extern int          SHELLMINATOR_WEAK neofetch_cpu_text_style;

extern const char*  SHELLMINATOR_WEAK neofetch_compiler_text;
extern int          SHELLMINATOR_WEAK neofetch_compiler_text_color;
extern int          SHELLMINATOR_WEAK neofetch_compiler_text_style;

extern const char*  SHELLMINATOR_WEAK neofetch_compile_date_text;
extern int          SHELLMINATOR_WEAK neofetch_compile_date_text_color;
extern int          SHELLMINATOR_WEAK neofetch_compile_date_text_style;

extern const char*  SHELLMINATOR_WEAK neofetch_author_text;
extern int          SHELLMINATOR_WEAK neofetch_author_text_color;
extern int          SHELLMINATOR_WEAK neofetch_author_text_style;

extern const char*  SHELLMINATOR_WEAK neofetch_user_field_1_label;
extern const char*  SHELLMINATOR_WEAK neofetch_user_field_1_text;
extern int          SHELLMINATOR_WEAK neofetch_user_field_1_text_color;
extern int          SHELLMINATOR_WEAK neofetch_user_field_1_text_style;

extern const char*  SHELLMINATOR_WEAK neofetch_user_field_2_label;
extern const char*  SHELLMINATOR_WEAK neofetch_user_field_2_text;
extern int          SHELLMINATOR_WEAK neofetch_user_field_2_text_color;
extern int          SHELLMINATOR_WEAK neofetch_user_field_2_text_style;

extern const char*  SHELLMINATOR_WEAK neofetch_user_field_3_label;
extern const char*  SHELLMINATOR_WEAK neofetch_user_field_3_text;
extern int          SHELLMINATOR_WEAK neofetch_user_field_3_text_color;
extern int          SHELLMINATOR_WEAK neofetch_user_field_3_text_style;

extern const char*  SHELLMINATOR_WEAK neofetch_user_field_4_label;
extern const char*  SHELLMINATOR_WEAK neofetch_user_field_4_text;
extern int          SHELLMINATOR_WEAK neofetch_user_field_4_text_color;
extern int          SHELLMINATOR_WEAK neofetch_user_field_4_text_style;

extern const char*  SHELLMINATOR_WEAK neofetch_user_field_5_label;
extern const char*  SHELLMINATOR_WEAK neofetch_user_field_5_text;
extern int          SHELLMINATOR_WEAK neofetch_user_field_5_text_color;
extern int          SHELLMINATOR_WEAK neofetch_user_field_5_text_style;

#endif