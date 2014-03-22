/*
//  main.c
//  Lab3
//
//  Created by Bryce Holton.
//  Copyright (c) 2014 Bryce
//	Holton. All rights reserved.
*/

#pragma warning(disable: 4996)
#include "common.h"
#include "print.h"
#include "scanner.h"

void add_token_to_list(struct Token *list, struct Token *new_token);
void quit_scanner(FILE *src_file, struct Token *list);
FILE *init_lister(const char *name, char source_file_name[], char dte[]);

int main(int argc, const char* argv[])
{
    struct Token *token;
    struct Token *token_list = NULL; /*This needs to be implemented as a linked list in scanner.h. */
    char source_name[MAX_FILE_NAME_LENGTH];
    char date[DATE_STRING_LENGTH];
    FILE *source_file;
	ch = &line_buffer;
	source_file = init_lister(argv[1], source_name, date);
    init_scanner(source_file, source_name, date);

    do
    {
        token = get_token();
        add_token_to_list(token_list, token);
        print_token(token);
    }
    while (strncmp(token->literalValue.valString, ".", 1) != 0);

    quit_scanner(source_file, token_list);


	/*==================================*/
	printf("DONE");
	getchar();
	/*==================================*/


    return 0;
}

void add_token_to_list(struct Token *list, struct Token *new_token)
{
    /*Add new_token to the list knowing that list is a linked list. */
	if(list == NULL){
		list = new_token;
	}else{
		new_token->next = NULL;
		list->next = new_token;
	}
}

void quit_scanner(FILE *src_file, struct Token *list)
{
    /*write code to free all of the memory for the token list */
	struct Token* ptr_lead;
	struct Token* ptr_trail;

	/* If list is empty */
	if(list == NULL){
		return;
	/* If list is size 1 */
	}else if(list->next == NULL){
		free(list);
		return;
	}

	/* If list is size >1 */
	ptr_lead = list->next;
	ptr_trail = ptr_lead;

	/* then iterate through and delete ptr_trail each time */
	while(ptr_lead != NULL){
		free(ptr_trail);
		ptr_trail = ptr_lead;
		ptr_lead = ptr_lead->next;
	}
	/* then finally delete last node in LinkedList */
	free(ptr_trail);

    fclose(src_file);
}

FILE *init_lister(const char *name, char source_file_name[], char dte[])
{
    time_t timer;
    FILE *file;

    strcpy(source_file_name, name);
    file = fopen(source_file_name, "r");
    time(&timer);
    strcpy(dte, asctime(localtime(&timer)));
    return file;
}

