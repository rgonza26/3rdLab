/*
//  print.c
//  Lab3
//
//  Created by Bryce Holton.
//  Copyright (c) 2014 Bryce Holton. All rights reserved.
 */

#include "print.h"

void print_line(char line[], char source_name_to_print[], char date_to_print[])
{
    char save_ch;
    char *save_chp = NULL;
    static int line_count = MAX_LINES_PER_PAGE;

    if (++line_count > MAX_LINES_PER_PAGE){
        print_page_header(source_name_to_print, date_to_print);
        line_count = 1;
    }if (strlen(line) > MAX_PRINT_LINE_LENGTH) {
        save_chp = &line[MAX_PRINT_LINE_LENGTH];
    }if (save_chp){
        save_ch = *save_chp;
        *save_chp = '\0';
    }
    printf("%s", line);
    if (save_chp){
        *save_chp = save_ch;
    }
}

static void print_page_header(char source_name[], char date[])
{
    static int page_number = 0;

    putchar(FORM_FEED_CHAR);
    printf("Page    %d  %s  %s\n\n", ++page_number, source_name, date);
}

void print_token(struct Token *token)
{
	if(token->literalType == STRING_LIT){
		printf("\t>> %s\t%s\n", SYMBOL_STRINGS[token->tokenCode], token->literalValue.valString);
	}else if(token->literalType == INTEGER_LIT){
		printf("\t>> %s\t%d\n", SYMBOL_STRINGS[token->tokenCode], token->literalValue.valInt);
	}else if(token->literalType == REAL_LIT){
		printf("\t>> %s\t%s\n", SYMBOL_STRINGS[token->tokenCode], token->literalValue.valString);
	}else{
		printf("\n\n\tERROR: Token->literalType INVALID\n\n");
		getchar();
	}
}















