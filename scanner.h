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

void init_scanner(FILE *source_file, char source_name[], char date[]);
BOOLEAN get_source_line(char source_buffer[]);
Token* get_token();
static char get_char();
static char peek_char();
static char skip_blanks();
static char skip_comment();
static Token get_word(char c);
static Token get_number(char c);
static Token get_string(char c);
static Token get_special(char c);
static void downshift_word(char* str);
static TokenCode is_reserved_word(char* str);


static FILE *src_file;
static char src_name[MAX_FILE_NAME_LENGTH];
static char todays_date[DATE_STRING_LENGTH];
static CharCode char_table[256];

static char* ch;
static char line_buffer[MAX_SOURCE_LINE_LENGTH];

typedef enum
{
    LETTER, DIGIT, QUOTE, SPECIAL, EOF_CODE, UNUSED
}
CharCode;

typedef struct
{
    char *string;
    TokenCode token_code;
}
RwStruct;

const RwStruct rw_table[9][10] = {
    /*[0]len2*/{{"do",DO},{"if",IF},{"in",IN},{"of",OF},{"or",OR},{"to",TO},{NULL,NO_TOKEN}}, 
    /*[1]len3*/{{"and",AND},{"div",DIV},{"end",END},{"for",FOR},{"mod",MOD},{"nil",NIL},{"not",NOT},{"set",SET},{"var",VAR},{NULL,NO_TOKEN}},
    /*[2]len4*/{{"case",CASE},{"else",ELSE},{"file",FFILE},{"goto",GOTO},{"then",THEN},{"type",TYPE},{"with",WITH},{NULL,NO_TOKEN}}, 
    /*[3]len5*/{{"array",ARRAY},{"begin",BEGIN},{"const",CONST},{"label",LABEL},{"until",UNTIL},{"while",WHILE},{NULL,NO_TOKEN}},  
    /*[4]len6*/{{"downto",DOWNTO}, {"packed",PACKED},{"record",RECORD}, {"repeat",REPEAT},{NULL,NO_TOKEN}},  
    /*[5]len7*/{{"program", PROGRAM},{NULL,NO_TOKEN}}, 
    /*[6]len8*/{{"function", FUNCTION},{NULL,NO_TOKEN}}, 
    /*[7]len9*/{{"procedure", PROCEDURE},{NULL,NO_TOKEN}}  
};


#endif
