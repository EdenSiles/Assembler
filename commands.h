#ifndef COMMANDS_H
#define COMMANDS_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**********************************************************************************************
This function creates an assembly code for an operation with 0 operands
* @param op- the value of the opertaion
* @return 0 if the creating succeed and -1 if there werw an error
 ********************************************************************************************/
int zero_operand(int command);
/**********************************************************************************************
This function creates an assembly code for an operation with 1 operands
* @param op- the value of the opertaion
* @return 0 if the creating succeed and -1 if there werw an error
 ********************************************************************************************/
int one_operand(int command);
/**********************************************************************************************
 This function creates an assembly code for an operation with 2 operands
 * @param op- the value of the opertaion
 * @return 0 if the creating succeed and -1 if there werw an error
  ********************************************************************************************/
int two_operand(int op);
/**********************************************************************************************
This function gets the addressing method of the operand by analizes the operand's string
* @param operand- the operand's string
* @return the number of the addressing method or -1 in case there is an error
 ********************************************************************************************/
int addressing_methods(char* operand);
/**********************************************************************************************
According to the operand's addressing method, this function calculates the operand value
* @param addressing- the addressing method of the operand
* @param operand - the index of data in which the operand's name is contained
* @return the value of the operand
 ********************************************************************************************/
int get_op_res(int addressing, int operand);
/**********************************************************************************************
This function search for an operation the matches the string and if it has found return its
funct field.
* @return the funct of the operand if the string matches one of the operations and -1 otherwise
 ********************************************************************************************/
int get_funct();
/**********************************************************************************************
This function calculate the register number
* @param operand- the operand's string
* @param ptr- will contains the charecter in this string which isn't a number ("r")
* @return the register number
 ********************************************************************************************/
int get_reg_num(char* operand, char** ptr);
/**********************************************************************************************
This function checks in the symbol tabel if the given operand is a label
* @param operand- the operand's string
* @return 1 if the operand is a label and 0 otherwise
 ********************************************************************************************/
int check_if_label(char* operand);

#endif