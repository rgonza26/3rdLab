/*
//  print.h
//  Lab3
//
//  Created by Bryce Holton.
//  Copyright (c) 2014 Bryce Holton. All rights reserved.
*/

#ifndef Lab3_print_h
#define Lab3_print_h

#include "common.h"

const char* const SYMBOL_STRINGS[] =
{
    "<no token>", "<IDENTIFIER>", "<NUMBER>", "<STRING>",
    "^","*","(",")","-","+","=","[","]",":",";",
    "<",">",",",".","/",":=","<=",">=","<>","..",
    "<END OF FILE>", "<ERROR>",
    "AND","ARRAY","BEGIN","CASE","CONST","DIV","DO","DOWNTO",
    "ELSE","END","FILE","FOR","FUNCTION","GOTO","IF","IN",
    "LABEL","MOD","NIL","NOT","OF","OR","PACKED","PROCEDURE",
    "PROGRAM","RECORD","REPEAT","SET","THEN","TO","TYPE","UNTIL",
    "VAR","WHILE","WITH"
};

<<<<<<< HEAD
void print_line(char line[], char source_name_to_print[], char date_to_print[]);
void print_page_header(char source_name[], char date[]);
=======

void print_line(char line[], char source_name_to_print[], char date_to_print[]);
static void print_page_header(char source_name[], char date[]);
>>>>>>> FETCH_HEAD
void print_token(struct Token *token);


#endif
