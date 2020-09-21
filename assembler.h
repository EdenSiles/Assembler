#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FIRST_ADDRESS 100
#define MAX_LINE_LEN 82
#define MAX_ARGS_LEN 120
#define MAX_LABEL_LEN 31 
#define OPERATIONS 16
#define DEFINITION 4

char *file_name;
int IC,DC,ICF,DCF; /*instruction and data counters*/
int extern_counter; /*extern symbols counters*/
char **data_arr; /*array of string arguments*/
char old_label[MAX_LABEL_LEN];

/*line_idx is the line number in the current file 
data_idx is the index of the current argument in data 
data_len is the length of the data array 
with_label is a flag which indicates if there is a label in the line*/
int line_idx, data_idx, data_len, with_label,op_idx;

/**********************************************************************************************
This function execute the first pass on the given file and search for syntax error
* @param fp- a pointer to the file
* @return 0 if the function finish the first pass in succes or 1 otherwise
 ********************************************************************************************/
int first_pass(FILE *fp);
/**********************************************************************************************
This function execute the second pass on the given file and search for syntax error. Moreover,
this function extracting labels data from the symbol tabel and inserting it to the machine code
* @param fp- a pointer to the file
* @return 0 if the function finish the second pass in succes or 1 otherwise
 ********************************************************************************************/
int second_pass(FILE *fp);
/**********************************************************************************************
This function prints the error messages to stderr.
* @param str - the error message
 ********************************************************************************************/
void p_errors(char *str);
/**********************************************************************************************
This function search for missing comma in the line only if it needed (not a string line or not
if it is an operation that need less than 2 operand.
 * @return -1 if there is a missimg comma and 0 otherwise
 ********************************************************************************************/
int missing_commas();
/**********************************************************************************************
This function search for illegal comma or multiple consecutive commas
 * @return -1 if there is illegal comma or multiple consecutive commas and 0 otherwise
 ********************************************************************************************/

int illegal_commas();
/**********************************************************************************************
This function check if the last of the line is smaller from the data length in 1.
 * @return -1 if there is extra text after end of command ens 0 otherwise
 ********************************************************************************************/
int extra_text();
/**********************************************************************************************
This function fress the memory allocation of the global structures
 ********************************************************************************************/
void frees_memory();
/**********************************************************************************************
This function removes all the white spaces from the given string
* @param str- the string from which the function remove all the white spaces
 ********************************************************************************************/
void remove_white_spaces(char *str);
/**********************************************************************************************
This function is an auxiliary function to skip white spaces by checking char
* @param c- the char which the function will check if it is a white space
 ********************************************************************************************/
int is_white_space(char c);
/**********************************************************************************************
This function is an auxiliary function which iterate through the white spaces at the beginning
of the string
* @param str- the string which will be checked
* @return the index of the first position which is not white space
 ********************************************************************************************/
int get_first_position(char *str);
/**********************************************************************************************
This function is an auxiliary function which iterate through the white spaces at the end
of the string
* @param str- the string which will be checked
* @return the index of the last position which is not white space
 ********************************************************************************************/
int get_last_position(char *str);
/**********************************************************************************************
This function calculate the length of the string without the white spaces
* @param str- the string which will be checked
* @return the correct length of a trimmed string
 ********************************************************************************************/
int get_trim_len(char *str);

#endif