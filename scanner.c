/*
//  scanner.c
//  Lab3
//
//  Created by Bryce Holton.
//  Copyright (c) 2014 Bryce Holton. All rights reserved.
*/

#include <stdio.h>
#include <string.h>
#include "scanner.h"

/*******************
 Static functions needed for the scanner
 You need to design the proper parameter list and 
 return types for functions with ???.
 ******************/
static char get_char();
static char peek_char();
static char skip_comment();
static char skip_blanks();
static Token get_word(char c);
static Token get_number(char c);
static Token get_string(char c);
static Token get_special(char  c);
static void downshift_word(char* str);
static TokenCode is_reserved_word(char* str);

typedef enum
{
    LETTER, DIGIT, QUOTE, SPECIAL, EOF_CODE, UNUSED
}
CharCode;

/*********************
 Static Variables for Scanner
 Must be initialized in the init_scanner function.
 *********************/
static FILE *src_file;
static char src_name[MAX_FILE_NAME_LENGTH];
static char todays_date[DATE_STRING_LENGTH];
static CharCode char_table[256];  /* The character table */
static char* ch;

static char line_buffer[MAX_SOURCE_LINE_LENGTH];

typedef struct
{
    char *string;
    TokenCode token_code;
}
RwStruct;

const RwStruct rw_table[9][10] = {
	/*	[0]	Reserved words of size 2	*/
    {{"do",DO},{"if",IF},{"in",IN},{"of",OF},{"or",OR},{"to",TO},{NULL,NO_TOKEN}}, 
	/*	[1]	Reserved words of size 3	*/
    {{"and",AND},{"div",DIV},{"end",END},{"for",FOR},{"mod",MOD},{"nil",NIL},{"not",NOT},{"set",SET},{"var",VAR},{NULL,NO_TOKEN}},
	/*	[2]	Reserved words of size 4	*/
    {{"case",CASE},{"else",ELSE},{"file",FFILE},{"goto",GOTO},{"then",THEN},{"type",TYPE},{"with",WITH},{NULL,NO_TOKEN}}, 
	/*	[3]	Reserved words of size 5	*/
    {{"array",ARRAY},{"begin",BEGIN},{"const",CONST},{"label",LABEL},{"until",UNTIL},{"while",WHILE},{NULL,NO_TOKEN}},  
	/*	[4]	Reserved words of size 6	*/
    {{"downto",DOWNTO}, {"packed",PACKED},{"record",RECORD}, {"repeat",REPEAT},{NULL,NO_TOKEN}},  
	/*	[5]	Reserved words of size 7	*/
    {{"program", PROGRAM},{NULL,NO_TOKEN}}, 
	/*	[6]	Reserved words of size 8	*/
    {{"function", FUNCTION},{NULL,NO_TOKEN}}, 
	/*	[7]	Reserved words of size 9	*/
    {{"procedure", PROCEDURE},{NULL,NO_TOKEN}}  
};

void init_scanner(FILE *source_file, char source_name[], char date[])
{
	int asciiChar;
    src_file = source_file;
    strcpy(src_name, source_name);
    strcpy(todays_date, date);
    
    /*******************
     initialize character table, this table is useful for identifying what type of character 
     we are looking at by setting our array up to be a copy the ascii table.  Since C thinks of 
     a char as like an int you can use ch in get_token as an index into the table.
     *******************/
	/*0,      1,     2,     3,       4,		   5		*/
    /*LETTER, DIGIT, QUOTE, SPECIAL, EOF_CODE, UNUSED	*/

	/*	Initialize array to UNUSED	*/
	for(asciiChar = 0; asciiChar <= 255; asciiChar++){
		char_table[asciiChar] = UNUSED;
	}
	/* [48, 57]	= DIGIT	*/
	for(asciiChar = 48; asciiChar<=57; asciiChar++){
		char_table[asciiChar] = DIGIT;
	}
	/* [65, 90] = LETTER */
	for(asciiChar = 65; asciiChar <= 90; asciiChar++){
		char_table[asciiChar] = LETTER;
	}
    /* [97, 122] = LETTER */
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
	/*	UNDERSCORE	*/
	char_table[95] = LETTER;
	/* QUOTES */
	char_table[39] = QUOTE;
}

BOOLEAN get_source_line(char source_buffer[])
{
    char print_buffer[MAX_SOURCE_LINE_LENGTH + 9];
/*  char source_buffer[MAX_SOURCE_LINE_LENGTH];  //I've moved this to a function parameter.  Why did I do that? */
    static int line_number = 0;
    
    if (fgets(source_buffer, MAX_SOURCE_LINE_LENGTH, src_file) != NULL)
    {
        ++line_number;
        sprintf(print_buffer, "%4d: %s", line_number, source_buffer);
        print_line(print_buffer, src_name, todays_date);
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}

Token* get_token()
{
	int iTest;
	float fTest;
	Token newToken;
    Token* retToken;  /*I am missing the most important variable in the function, what is it?  Hint: what should I return? */
    char c; /*This can be the current character you are examining during scanning. */
	BOOLEAN isReserved;
    retToken = (Token*)malloc(sizeof(Token));

    /*	1.  Skip past all of the blanks	*/
	skip_blanks();
	if(peek_char() == '{'){
		c = skip_comment();
	}
    /*	2.  figure out which case you are dealing with LETTER, DIGIT, QUOTE, EOF, or SPECIAL, by examining ch	*/
	if(char_table[c] == LETTER){
		newToken = get_word(c);
	}else if(char_table[c] == DIGIT){
		newToken = get_number(c);
	}else if(char_table[c] == QUOTE){
		newToken = get_string(c);
	}else if(c == EOF){
		newToken.literalValue.valInt = -1;
		newToken.literalType = INTEGER_LIT;
		newToken.tokenCode = END_OF_FILE;
	}else if(char_table[c] == SPECIAL){
		newToken = get_special(c);
	}

	memcpy(retToken, &newToken, sizeof(Token));
    return retToken; /*What should be returned here? */
}

static char get_char()
{	
    /*
     If at the end of the current line (how do you check for that?),
     we should call get source line.  If at the EOF (end of file) we should
     set the character ch to EOF and leave the function.
     */
	char c;
	if(*ch == EOF){
		return EOF;
	/*	NULLCHAR OR NEWLINE OR > ? ? ?	*/
	}else if(*ch = '\0'){
		get_source_line(line_buffer);
		ch = line_buffer;
	}
	c = *ch;
	ch++;
	return c;
    /*
     Write some code to set the character ch to the next character in the buffer
     */
}

static char peek_char(){
	return *ch;
}


static char skip_blanks()
{
    /*
     Write some code to skip past the blanks in the program and return a pointer
     to the first non blank character
     */
	char c;
	do{
		c = get_char();
	}while(c == ' ');
	return c;
}

static char skip_comment()
{
    /*
     Write some code to skip past the comments in the program and return a pointer
     to the first non blank character.  Watch out for the EOF character.
     */
	char c;

	do{
		c = get_char();
	}while(c != '}');

	c = get_char();

	if(c == ' '){
		c = skip_blanks();
	}if(c == '{'){
		return skip_comment();
	}else{
		return c;
	}
}

static Token get_word(char c)
{
	/*	Stores word in char buffer[]	*/
    /*
     Write some code to Extract the word
     */
	Token token;
	LiteralValue toLower;

	token.literalType = STRING_LIT;
	token.literalValue.valString[0] = c;
	int i = 1;
	while(char_table[peek_char()] == LETTER || char_table[peek_char()] == NUMBER){
		c = get_char();
		token.literalValue.valString[i] = toLower.valString[i] = c;
		i++;
	}
	while(i < MAX_TOKEN_STRING_LENGTH){
		token.literalValue.valString[i] = toLower.valString[i] = '\0';
		i++;
	}

	downshift_word(toLower.valString);
	token.tokenCode = is_reserved_word(token.literalValue.valString);

	return token;
    /*
     Write some code to Check if the word is a reserved word.
     if it is not a reserved word its an identifier.
     */
}

static Token get_number(char c)
{
    /*
     Write some code to Extract the number and convert it to a literal number.
     */
	Token token;
	BOOLEAN hasDecimal;
	int i = 0;

	token.tokenCode = NUMBER;
	token.literalValue.valString[0] = c;

	while(char_table[peek_char()] == LETTER || char_table[peek_char()] == NUMBER || c == '.'){
		c = get_char();
		token.literalValue.valString[i] = c;
		hasDecimal = (c == '.')? TRUE : FALSE;
		i++;
	}

	for(i; i<MAX_TOKEN_STRING_LENGTH; i++){
		token.literalValue.valString[i] = '\0';
	}

	if(hasDecimal){
		token.literalType = REAL_LIT;
		token.literalValue.valDouble = atof(token.literalValue.valString);
	}else{
		token.literalType = INTEGER_LIT;
		token.literalValue.valInt = atoi(token.literalValue.valString);
	}

	/*
	LiteralValue retVal = {(hasDecimal? atof(token.literalValue.valString) : atoi(token.literalValue.valString))};
	*/

	return token;
}

static Token get_string(char c)
{
    /*
     Write some code to Extract the string
     */
	Token token;
	int i = 0;

	token.literalType = STRING_LIT;
	token.tokenCode = STRING;
	token.literalValue.valString[0] = c;

	while(peek_char() != '\''){
		c = get_char();
		token.literalValue.valString[i] = c;
		i++;
	}
	for(i; i<MAX_TOKEN_STRING_LENGTH; i++){
		token.literalValue.valString[i] = '\0';
	}
	
}

static Token get_special(char c)
{
    /*
     Write some code to Extract the special token.  Most are single-character
     some are double-character.  Set the token appropriately.
     */
	Token token;
	int i;

	token.literalType = STRING_LIT;

	token.literalValue.valString[0] = c;
	token.literalValue.valString[1] = (char_table[peek_char()] == SPECIAL)? get_char() : '\0';
	token.literalValue.valString[2] = '\0';

	token.tokenCode = is_reserved_word(token.literalValue.valString);

	return token;
}

static void downshift_word(char* str)
{
	/* This function takes a pointer to a string, and manipulates THAT string
		to make all characters lowercase.  Void return	*/

	/* LOCAL DECLARATIONS */
	char* ptr = str;
	/* Adding shamt=32 to a an uppercase char makes it lowercase */
	int shamt = 32;

	/* FUNC BODY */
	/* Iterate until null character '\0' found */
	while(*ptr!='\0'){
		/* If character is an uppercase letter, make it lowercase by adding shamt */
		if(*ptr >= 65 && *ptr <= 90){
			*ptr = *ptr + shamt;
		}
		ptr++; 
	}
}

static TokenCode is_reserved_word(char* str)
{
    /*	Examine the reserved word table and determine if the function input is a reserved word.	*/
	char* strPtr;
	int i = 0, strLength = strlen(str);
	if(strLength < 2 || strLength > 9){
		/*	All tokens are of length [2,9] so if str isn't in that range, return false.	*/
		return NO_TOKEN;
	}

	do{
		/*	Get char* to next token	*/
		strPtr = rw_table[i][strLength-2].string;
		if(strncmp(str, strPtr, strLength) == 0){
			/*	If strings are equal return the TokenCode for that reserved word	*/
			return rw_table[i][strLength-2].token_code;
		}
		i++;
	}while(strPtr != NULL);
	return NO_TOKEN;
}
