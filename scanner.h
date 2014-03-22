/*
//  scanner.h
//  Lab3
//
//  Created by Bryce Holton.
//  Copyright (c) 2014 Bryce Holton. All rights reserved.
*/

#ifndef Lab3_scanner_h
#define Lab3_scanner_h

#include "common.h"
#include "print.h"

typedef enum
{
    LETTER, DIGIT, QUOTE, SPECIAL, EOF_CODE, UNUSED
}
CharCode;

struct RwStruct
{
    char *string;
    TokenCode token_code;
};

void init_scanner(FILE *source_file, char source_name[], char date[]);
BOOLEAN get_source_line(char source_buffer[]);
struct Token* get_token();
char get_char();
char peek_char();
char skip_blanks();
char skip_comment();
struct Token get_word(char c);
struct Token get_number(char c);
struct Token get_string(char c);
struct Token get_special(char c);
void downshift_word(char* str);
TokenCode is_reserved_word(char* str);


static FILE *src_file;
static char src_name[MAX_FILE_NAME_LENGTH];
static char todays_date[DATE_STRING_LENGTH];
static CharCode char_table[256];

static char* ch;
static char line_buffer[MAX_SOURCE_LINE_LENGTH];

#endif
