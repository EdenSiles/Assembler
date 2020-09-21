#ifndef CHECK_LINE_H
#define CHECK_LINE_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**********************************************************************************************
This function check the given line and search for string, commas and white characters.
* @param line- the assembly line
* @return value is the number of opernd in the line or -1 in case illegal string argument
 ********************************************************************************************/
int check_line(char* line);
/**********************************************************************************************
This function copies a string in case the string exist
* @param line- the assembly line
* @param str- the string will copy to this variable in if exists
* @param ptr- a pointer to the begining of the string
* @return 0 if the copy success or -1 if the is syntax error
 ********************************************************************************************/
int get_str(char* line, char* str, char* ptr);
/**********************************************************************************************
This function checks an argument in the line
* @param word- the argument that the function is going to check
* @param idx- the current number of arguments
* @return the number of argument after checking the word. In case there is a comment in the word,
	the function return -1.
 ********************************************************************************************/
int check_word(char* word, int idx);

#endif