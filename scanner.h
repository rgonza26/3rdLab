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

void init_scanner(FILE* source_file, char source_name[], char date[]);
Token* get_token();

#endif
