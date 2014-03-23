/*
//  scanner.c
//  Lab3
//
//  Created by Bryce Holton.
//  Copyright (c) 2014 Bryce Holton. All rights reserved.
*/

#pragma warning(disable: 4996)
#include <stdio.h>
#include <string.h>

#include "scanner.h"

const struct RwStruct rw_table[9][15] = {
	/*[0]len1*/{{"^",UPARROW},{"*",STAR},{"(",LPAREN},{")",RPAREN},{"-",MINUS},{"[",LBRACKET},{"]",RBRACKET},{":",COLON},{";",SEMICOLON},{"<",LT},{">",GT},{",",COMMA},{".",PERIOD},{"/",SLASH},{NULL,NO_TOKEN}},
	/*[1]len2*/{{"do",DO},{"if",IF},{"in",IN},{"of",OF},{"or",OR},{"to",TO},{":=",COLONEQUAL},{"<=",LE},{">=",GE},{"<>",NE},{"..",DOTDOT},{NULL,NO_TOKEN}},
    /*[2]len3*/{{"and",AND},{"div",DIV},{"end",END},{"for",FOR},{"mod",MOD},{"nil",NIL},{"not",NOT},{"set",SET},{"var",VAR},{NULL,NO_TOKEN}},
    /*[3]len4*/{{"case",CASE},{"else",ELSE},{"file",FFILE},{"goto",GOTO},{"then",THEN},{"type",TYPE},{"with",WITH},{NULL,NO_TOKEN}},
    /*[4]len5*/{{"array",ARRAY},{"begin",BEGIN},{"const",CONST},{"label",LABEL},{"until",UNTIL},{"while",WHILE},{NULL,NO_TOKEN}},
    /*[5]len6*/{{"downto",DOWNTO}, {"packed",PACKED},{"record",RECORD}, {"repeat",REPEAT},{NULL,NO_TOKEN}},
    /*[6]len7*/{{"program", PROGRAM},{NULL,NO_TOKEN}},
    /*[7]len8*/{{"function", FUNCTION},{NULL,NO_TOKEN}},
    /*[8]len9*/{{"procedure", PROCEDURE},{NULL,NO_TOKEN}}
};

void init_scanner(FILE *source_file, char source_name[], char date[])
{
	int asciiChar;
    src_file = source_file;
    strcpy(src_name, source_name);
    strcpy(todays_date, date);

	/*0,      1,     2,     3,       4,		   5		*/
    /*LETTER, DIGIT, QUOTE, SPECIAL, EOF_CODE, UNUSED	*/
	for(asciiChar = 0; asciiChar <= 255; asciiChar++){
		char_table[asciiChar] = UNUSED;
	}
	for(asciiChar = 48; asciiChar<=57; asciiChar++){
		char_table[asciiChar] = DIGIT;
	}
	for(asciiChar = 65; asciiChar <= 90; asciiChar++){
		char_table[asciiChar] = LETTER;
	}
	for(asciiChar = 97; asciiChar <= 122; asciiChar++){
		char_table[asciiChar] = LETTER;
	}
	/*	SPECIAL CHARACTERS:
	^ * ( ) - + = [ ] : ; < > , . / := <= >= <> ..
	*/
	for(asciiChar = 40; asciiChar <= 47; asciiChar++){
		char_table[asciiChar] = SPECIAL;
	}
	for(asciiChar = 58; asciiChar <= 62; asciiChar++){
		char_table[asciiChar] = SPECIAL;
	}
	for(asciiChar = 91; asciiChar <= 94; asciiChar++){
		char_table[asciiChar] = SPECIAL;
	}
	char_table[95] = LETTER;	/* _underscore_	*/
	char_table[39] = QUOTE;
	char_table[32] = UNUSED;	/*	' space '	*/

	get_source_line(line_buffer);
}

BOOLEAN get_source_line(char source_buffer[])
{
    char print_buffer[MAX_SOURCE_LINE_LENGTH + 9];
    static int line_number = 0;

    if (fgets(source_buffer, MAX_SOURCE_LINE_LENGTH, src_file) != NULL){
        ++line_number;
        sprintf(print_buffer, "%4d: %s", line_number, source_buffer);
        print_line(print_buffer, src_name, todays_date);
		printf("\n");
        return TRUE;
    }else{
        return FALSE;
    }
}

struct Token* get_token()
{
	struct Token newToken;
    struct Token* retToken;
    char c = '\0';
    retToken = (struct Token*)malloc(sizeof(struct Token));

	c = skip_blanks();
	if(peek_char() == '{'){
		c = skip_comment();
	}

	if(char_table[c] == LETTER){
		newToken = get_word(c);
	}else if(char_table[c] == DIGIT){
		newToken = get_number(c);
	}else if(char_table[c] == QUOTE){
		newToken = get_string(c);
	}else if(c == EOF){
		newToken.literalValue.valString[0] = '.';
		newToken.literalType = INTEGER_LIT;
		newToken.tokenCode = END_OF_FILE;
	}else if(char_table[c] == SPECIAL){
		newToken = get_special(c);
	}

	memcpy(retToken, &newToken, sizeof(struct Token));
    return retToken;
}

char get_char()
{
	char c;
	if(*ch == EOF){
		return EOF;
	}else if(*ch == '\n' || *ch == '\0'){
		get_source_line(line_buffer);
		ch = &line_buffer;
	}
	c = *ch;
	ch++;
	return c;
}

char peek_char(){
	return *ch;
}


char skip_blanks()
{
	char c;
	do{
		c = get_char();
	}while(c == ' ' || c == '\n' || c == '\0' || c == '\t'|| c == '\r');
	return c;
}

char skip_comment()
{
	char c;

	do{
		c = get_char();
	}while(c != '}');

	c = get_char();

	if(c == ' ' || c == '\n' || c == '\0'){
		c = skip_blanks();
	}if(c == '{'){
		return skip_comment();
	}else{
		return c;
	}
}

struct Token get_word(char c)
{
	struct Token token;
	TokenCode code;
	int i = 1;

	token.literalType = STRING_LIT;
	token.literalValue.valString[0] = c;
	while(char_table[peek_char()] == LETTER || char_table[peek_char()] == NUMBER){
		c = get_char();
		token.literalValue.valString[i] = c;
		i++;
	}
	while(i < MAX_TOKEN_STRING_LENGTH){
		token.literalValue.valString[i] = '\0';
		i++;
	}

	downshift_word(token.literalValue.valString);
	code = is_reserved_word(token.literalValue.valString);
	token.tokenCode = (code == NO_TOKEN)? IDENTIFIER : code;

	return token;
}

struct Token get_number(char c)
{
	struct Token token;
	BOOLEAN isReal = FALSE;
	int i = 1;
	

	token.tokenCode = NUMBER;
	token.literalValue.valString[0] = c;

	while(char_table[peek_char()] == DIGIT || peek_char() == '.' || peek_char() == 'e'|| peek_char() == '-'){
		c = get_char();
		token.literalValue.valString[i] = c;
		isReal = (c == '.' || c == 'e' || c == '-')? TRUE : isReal;
		i++;
	}

	for(i; i<MAX_TOKEN_STRING_LENGTH; i++){
		token.literalValue.valString[i] = '\0';
	}

	if(isReal){
		token.literalType = REAL_LIT;
	}else{
		token.literalType = INTEGER_LIT;
		token.literalValue.valInt = atoi(token.literalValue.valString);
	}

	return token;
}

struct Token get_string(char c)
{
	struct Token token;
	int i = 0;

	token.literalType = STRING_LIT;
	token.tokenCode = STRING;
	token.literalValue.valString[0] = c;

	while(char_table[c = get_char()] != QUOTE){
		token.literalValue.valString[i] = c;
		i++;
	}
	for(i; i<MAX_TOKEN_STRING_LENGTH; i++){
		token.literalValue.valString[i] = '\0';
	}
	return token;
}

struct Token get_special(char c)
{
	struct Token tokenOneChar, tokenTwoChar, tokenFinal;
	TokenCode codeOneChar, codeTwoChar, codeFinal;

	tokenOneChar.literalType = STRING_LIT;
	tokenTwoChar.literalType = STRING_LIT;


	tokenOneChar.literalValue.valString[0] = c;
	tokenOneChar.literalValue.valString[1] = '\0';

	tokenTwoChar.literalValue.valString[0] = c;
	tokenTwoChar.literalValue.valString[1] = peek_char();
	tokenTwoChar.literalValue.valString[2] = '\0';

	codeOneChar = is_reserved_word(tokenOneChar.literalValue.valString);
	codeTwoChar = is_reserved_word(tokenTwoChar.literalValue.valString);

	if(codeTwoChar != NO_TOKEN){
		get_char();
		codeFinal = codeTwoChar;
		tokenFinal = tokenTwoChar;
	}else{
		codeFinal = codeOneChar;
		tokenFinal = tokenOneChar;
	}
	tokenFinal.tokenCode = codeFinal;

	return tokenFinal;
}

void downshift_word(char* str)
{
	int shamt = 32;
	char* ptr = str;

	while(*ptr!='\0'){
		if(*ptr >= 65 && *ptr <= 90){
			*ptr = *ptr + shamt;
		}
		ptr++;
	}
}

TokenCode is_reserved_word(char* str)
{
	char* strPtr;
	long strLength;
	int i = 0;
	strLength = strlen(str);

	if(strLength > 9){return NO_TOKEN;}
	strPtr = rw_table[strLength-1][i].string;
	while(strPtr != NULL){
		if(strncmp(str, strPtr, strLength) == 0){
			return rw_table[strLength-1][i].token_code;
		}
		i++;
		strPtr = rw_table[strLength-1][i].string;
	}

	return NO_TOKEN;
}
