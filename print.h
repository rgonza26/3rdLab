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

void print_line(char line[], char source_name_to_print[], char date_to_print[]);
void print_page_header(char source_name[], char date[]);
void print_token(struct Token *token);


#endif
