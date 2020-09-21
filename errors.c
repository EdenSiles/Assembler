#include "config.h"
#include "assembler.h"
#include "labels.h"
#include "def_op.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


 /**********************************************************************************************
 This function prints the error messages to stderr.
 * @param str - the error message
  ********************************************************************************************/
void p_errors(char* str) {
	fprintf(stdout, "Error in file:%s, line number :%d: %s\n", file_name, line_idx, str);
}

/**********************************************************************************************
This function search for missing comma in the line only if it needed (not a string line or not 
if it is an operation that need less than 2 operand.
 * @return -1 if there is a missimg comma and 0 otherwise
 ********************************************************************************************/
int missing_commas() {
	int i,res;
	for (i = 0; i < data_len; i++) { /*scaning the line and check if we expected a comma*/
		if ((!strcmp(data_arr[i], ".string")) || ((get_op() >= 5) && (get_op() < 15)) || ((!strcmp(data_arr[i], ".data") && (data_len-(i+1)==1)))) {
			return 0;
		}
	}
	/*in case we expected a comma, check if the labes is valid and determine the variable by the result*/
	res = legal_label(*data_arr);
	if (res) {	
		if (res == -1) {
			p_errors("Invalid label- the label is too long");
			return -1;
		}
		i = 3;
	}
	else {		
		i = 2;
	}
	/*search in the line for missing comma*/
	for (; i < data_len; i += 2) {
		if (strcmp(data_arr[i], ",")) {
			p_errors("Missing comma");
				return -1;
			}
		}
	return 0;
}

/**********************************************************************************************
This function search for illegal comma or multiple consecutive commas
 * @return -1 if there is illegal comma or multiple consecutive commas and 0 otherwise
 ********************************************************************************************/
int illegal_commas() {
	int i = 1;
	int tmp1 = 0, tmp2 = 0;
	int label;
	label = legal_label(*data_arr); /*check if the labes is valid*/
	if (label == -1) {
		p_errors("Invalid label- the label is too long");
		return -1;
	}
	while (i < data_len) {
		if ((strcmp(data_arr[i - 1], ",") == 0) && (strcmp(data_arr[i], ",") == 0)) { /*check if there is 2 commas in a row*/
			p_errors("Multiple consecutive commas"); 
			return -1;
		}
		i++;
	}
	/*check if the is a comma before the first argument*/
	if (data_len > 2) {
		tmp1 = !strcmp(data_arr[1], ","); 
		if (label) {
			tmp2 = !strcmp(data_arr[2], ",");
		}
		else
			tmp2 = 0;
	}
	else
		tmp1 = 0;
	if (!strcmp(data_arr[0], ",") || tmp1 || tmp2) { 
		p_errors("Illegal comma");
		return -1;
	}
	return 0;
}

/**********************************************************************************************
This function check if the last of the line is smaller from the data length in 1. 
 * @return -1 if there is extra text after end of command ens 0 otherwise
 ********************************************************************************************/
int extra_text() {
	/*the index of the last argument of the valid line should be 1 less than the data length */
	if (data_idx < data_len - 1) {
		p_errors("Extra text after end of command");
		return -1;
	}
	else
		return 0;
}