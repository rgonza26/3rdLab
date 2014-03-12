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
static char* skip_comment(char* str);
static char* skip_blanks(char* str);
static ??? get_word(???);
static ??? get_number(???);
static ??? get_string(???);
static ??? get_special(???);
static void downshift_word(char* str);
static BOOLEAN is_reserved_word(char* str);

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

typedef struct
{
    char *string;
    TokenCode token_code;
}
RwStruct;

const RwStruct rw_table[9][10] = {
	/*	[0]	Reserved words of size 2	*/
    {{"do",DO},{"if",IF},{"in",IN},{"of",OF},{"or",OR},{"to",TO},{NULL,0}}, 
	/*	[1]	Reserved words of size 3	*/
    {{"and",AND},{"div",DIV},{"end",END},{"for",FOR},{"mod",MOD},{"nil",NIL},{"not",NOT},{"set",SET},{"var",VAR},{NULL,0}},
	/*	[2]	Reserved words of size 4	*/
    {{"case",CASE},{"else",ELSE},{"file",FFILE},{"goto",GOTO},{"then",THEN},{"type",TYPE},{"with",WITH},{NULL,0}}, 
	/*	[3]	Reserved words of size 5	*/
    {{"array",ARRAY},{"begin",BEGIN},{"const",CONST},{"label",LABEL},{"until",UNTIL},{"while",WHILE},{NULL,0}},  
	/*	[4]	Reserved words of size 6	*/
    {{"downto",DOWNTO}, {"packed",PACKED},{"record",RECORD}, {"repeat",REPEAT},{NULL,0}},  
	/*	[5]	Reserved words of size 7	*/
    {{"program", PROGRAM},{NULL,0}}, 
	/*	[6]	Reserved words of size 8	*/
    {{"function", FUNCTION},{NULL,0}}, 
	/*	[7]	Reserved words of size 9	*/
    {{"procedure", PROCEDURE},{NULL,0}}  
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

	/* QUOTES */
	char_table[34] = QUOTE;
	char_table[39] = QUOTE;
	char_table[96] = QUOTE;
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
    char ch; /*This can be the current character you are examining during scanning. */
    char token_string[MAX_TOKEN_STRING_LENGTH]; /*Store your token here as you build it. */
    char *token_ptr = token_string; /*write some code to point this to the beginning of token_string */
    Token token;  /*I am missing the most important variable in the function, what is it?  Hint: what should I return? */
    
    /*1.  Skip past all of the blanks

    //2.  figure out which case you are dealing with LETTER, DIGIT, QUOTE, EOF, or special, by examining ch
    //3.  Call the appropriate function to deal with the cases in 2. */
    
    return ???; /*What should be returned here? */
}

static char get_char()// still testing...
{
    char ch = EOF_CHAR;
    
 	if (get_source_line[line_number] == '\n' ||
        line_number >= MAX_SOURCE_LINE_LENGTH) {
        
 		ch = '\0';
	}
    
	else
		ch = get_source_line[line_number];
    
	line_number++;
	return ch;	
    /*
     If at the end of the current line (how do you check for that?),
     we should call get source line.  If at the EOF (end of file) we should
     set the character ch to EOF and leave the function.
     */
    
    /*
     Write some code to set the character ch to the next character in the buffer
     */
}

static char* skip_blanks(char* str)
{
    /*
     Write some code to skip past the blanks in the program and return a pointer
     to the first non blank character
     */
	char* ptr = str;
	while(*ptr == ' '){
		ptr++;
	}
	return ptr;
}

static char* skip_comment(char* str)
{
    /*
     Write some code to skip past the comments in the program and return a pointer
     to the first non blank character.  Watch out for the EOF character.
     */
	char* ptr = str;
	do{
		ptr = skip_blanks(ptr);
		if(*str == '{'){
			while(*str != '}'){
				ptr++;
			}
			ptr++;
		}
		skip_blanks(ptr);
	}while(*ptr == '{' || *ptr == ' ');

	return ptr;
}

static ??? get_word(???)
{
    /*
     Write some code to Extract the word
     */
    
    //Downshift the word, to make it lower case
    
    /*
     Write some code to Check if the word is a reserved word.
     if it is not a reserved word its an identifier.
     */
}

static ??? get_number(???)
{
    /*
     Write some code to Extract the number and convert it to a literal number.
     */
}

static ??? get_string(???)
{
    /*
     Write some code to Extract the string
     */
}

static ??? get_special(???)
{
    /*
     Write some code to Extract the special token.  Most are single-character
     some are double-character.  Set the token appropriately.
     */
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

static BOOLEAN is_reserved_word(char* str)
{
    /*	Examine the reserved word table and determine if the function input is a reserved word.	*/
	char* strPtr;
	int i = 0, strLength = strlen(str);
	if(strLength < 2 || strLength > 9){
		/*	All tokens are of length [2,9] so if str isn't in that range, return false.	*/
		return FALSE;
	}

	do{
		/*	Get char* to next token	*/
		strPtr = rw_table[i][strLength-2].string;
		if(strncmp(str, strPtr, strLength) == 0){
			/*	If strings are equal...
			...return TRUE	*/
			return TRUE;
		}
		i++;
	}while(strPtr != NULL);

	return FALSE;

}
