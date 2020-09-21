#ifndef DEF_OP_H
#define DEF_OP_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**********************************************************************************************
This function insert the data of the instructions to the data table
* @param label- contains the label which matches to the definition (in case there is a label)
* @return -1 if the is an error in the line or 0 otherwise
 ********************************************************************************************/
int insert_data(char* label);
/**********************************************************************************************
 This function converts the assembly instruction line to binary code in the instruction table
 * @param label- contains the label which matches to the instruction (in case there is a label)
 * @return -1 if the is an error in the line or 0 otherwise
  ********************************************************************************************/
int insert_instruction(char* label);
/**********************************************************************************************
This function adds the data to data table when it is a line with data or string definition
* @param type_val- the type of the definition
* @return -1 if the is an error in the line or 0 otherwise
 ********************************************************************************************/
int add_to_data_table(int type_val);
/**********************************************************************************************
This function search for the defintion of the line
* @return the definition value if the function found it or -1 otherwise
 ********************************************************************************************/
int get_def();
/**********************************************************************************************
This function search for the operation of the line
* @return the operation if the function found it or -1 otherwise
 ********************************************************************************************/
int get_op();
/**********************************************************************************************
This function check if ths value is valid by checking the range if the value
* @param str- the string which contains the value
* @return 2 if the value has up to 21 bits, 1 if the value between 0 and 24 and 0 otherwise
 ********************************************************************************************/
int is_valid_val(char* str);

#endif