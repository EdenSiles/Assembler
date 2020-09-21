#ifndef FILES_H
#define FILES_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**********************************************************************************************
This function opens an assembly file and calls the functions that make the first and second pass.
In case there were no errors, the function creates the output files.
 * @param argc - the amount of assembly files
 * @param argv - the assembly files
  ********************************************************************************************/
void open_files(int argc, char* argv[]);
/**********************************************************************************************
This function creates a file with write permitions.
In case there were no errors, the function creates the output files.
Input parameters:
* @param - the name of the new file
* @param- the end of the file (".ob"/".ent"/".ext")
* @return a pointer to the new file
 ********************************************************************************************/
FILE* create_file(char* str, char* end);
/**********************************************************************************************
This function create a file with ".ext" ending and write in this file the extern labels
and their addresses.
Input parameters:
	# str - the file name (without the ".as" ending)
Returns:
	# nothing - void function
 ********************************************************************************************/
void create_ext_file(char* str);
/**********************************************************************************************
This function create a file with ".ob" ending and write in this file the instruction and data
words and their addresses.
* @param str - the file name (without the ".as" ending)
 ********************************************************************************************/
void create_ob_file(char* str);
/**********************************************************************************************
This function create a file with ".ent" ending and write in this file the entry labels
and their addresses.
 * @param str - the file name (without the ".as" ending)
 ********************************************************************************************/
void create_ent_file(char* str);
/**********************************************************************************************
This function creates all the output files
* @param str - the file name (without endings)
 ********************************************************************************************/
void create_output_files(char* str);

#endif