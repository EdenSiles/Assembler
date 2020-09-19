#ifndef LABELS_H
#define LABELS_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/**********************************************************************************************
This function check if the label is legal (there is ":" in the end of the label)
* @param str- the label that the function will check
* @return 1 if there is ":" in  the label and otherwise return 0
********************************************************************************************/
int legal_label(char* str);
/**********************************************************************************************
If a label exist, extract the label from the data_arr and copy it to the string
 * @param str- the string will copy to this variable in if exists
 * @param in_second_pass- a flag which indicates id we are in the second pass
 * @return 0 if the copy success or -1 if label is invalid
  ********************************************************************************************/
int extract_insert_label(char* str, int in_second_pass);
/**********************************************************************************************
This function checks if the label is valid ot not
* @param line- the label the function will check
* @return 0 if the label is valid and -1 otherwise
 ********************************************************************************************/
int invalid_label(char* str);
/**********************************************************************************************
This function adds a label to symbol table
* @param str- the label
* @param type- the type of the label
* @param val- the value of the symbol in decimal. It will be the address of the symbol
* @return -1 if there is another definition of the label and otherwise return 0
 ********************************************************************************************/
int add_label(char* str, int type, int val);
/**********************************************************************************************
This function adds (when needed) entry to the typpe of the symbol
* @param str- the symbol to which we will add the enty type
* @return -1 if the symbol isn't defined or 0 otherwise
 ********************************************************************************************/
int add_entry_to_symbol(char* str);
/**********************************************************************************************
This function gets the address of wanted symbol
* @param str- the symbol we wand his address
* @return the address of the symbol
 ********************************************************************************************/
int get_address_of_symbol(char* str);
/**********************************************************************************************
This function adds IC to the address of each symbol with type DATA
 ********************************************************************************************/
void incr_address();
/**********************************************************************************************
This function checks if the label is valid (not operation and not a definition)
* @param str- the string which will checked
* @param arr- the array in which the function search for the label
* @param size- the length of the array
* @return -1 in case the string is an operation or definition (invalid label) and 0 otherwise
 ********************************************************************************************/
int check_labels(char* str, op *arr, int size);
/**********************************************************************************************
This function adds the address of the symbol to the matching place in the symbol tabel
* @return -1 if the symbol wasn't defimed and 0 in case the adding succed
 ********************************************************************************************/
int add_address_to_symblos();
/**********************************************************************************************
In case of extern fuction, this function adds an aooearance to the extern array structure
* @param str- the symbol's name
* @param val- the value of the symbol (his address)
 ********************************************************************************************/
void add_to_extern_arr(char* str, int val);
/**********************************************************************************************
In case of relative addressing method, this calculate the distance between the symbols addersses
and set the extra word
* @param str- the symbol's name
 ********************************************************************************************/
void add_relative_addressing(char *str);

#endif