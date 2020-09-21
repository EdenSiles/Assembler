#include "config.h"
#include "assembler.h"
#include "def_op.h"
#include "commands.h"
#include "labels.h"


 /**********************************************************************************************
 This function insert the data of the instructions to the data table
 * @param label- contains the label which matches to the definition (in case there is a label)
 * @return -1 if the is an error in the line or 0 otherwise
  ********************************************************************************************/
int insert_data(char *label) {
	int definition = get_def(); /*get the defintion number*/

	if (definition == -1) {
		if (data_arr[data_idx][0] == '.') { /*if the first char is '.' it is undefined definition*/
			p_errors("Undefined definition");
			return -1;
		}
		return 1;
	}
	if (data_arr[data_idx + 1] == NULL) { 
		p_errors("Missing operand after definition");
		return -1;
	}
	if (with_label == -1) {
		data_idx++;
		if ((data_arr[data_idx][0] == '.') || (get_op() != -1)) {
			data_idx--;
			p_errors("Invalid symbol");
			return -1;
		}
		data_idx--;
	}

	data_idx++;
	if (*label) { 
		if ((definition == EXTERN) || (definition == ENTRY)) {
			fprintf(stdout, "Warning: file: %s, line: %d - label before extern definition\n", file_name, line_idx);		
		}
		else {
			if (add_label(label, DATA, DC)) /*adding the label that matched to data or string definitions to the symbol table*/
				return -1;
		}
	}
	if (definition != ENTRY) { /*entry definition will handle in second pass*/
		if (definition != EXTERN) {
			return add_to_data_table(definition);  /*if the definition isn't extern, add the data to data table*/
		}
		if (add_label(data_arr[data_idx], EXTERN, 0)) /*add the label to symbol tabel*/
			return -1;
	} 
	return 0;
}

 /**********************************************************************************************
 This function converts the assembly instruction line to binary code in the instruction table
 * @param label- contains the label which matches to the instruction (in case there is a label)
 * @return -1 if the is an error in the line or 0 otherwise
  ********************************************************************************************/
int insert_instruction(char *label) {
	int operation = get_op();  /*getting the operation number */
	if (with_label == -1){
		data_idx++;
		if ((data_arr[data_idx][0] == '.') || (get_op() != -1)) {
			data_idx--;
			p_errors("Invalid symbol");
			return -1;
		}
		data_idx--;
	}
	if (operation == -1) {
		p_errors("Undefined instruction name");
		return -1;
	}

	if (*label) /*add the label to the symbol table in case the labek exist */
		add_label(label, CODE, IC);

	/*execute operation by opcode value*/
	if (operation < 14) {
		data_idx++;
		if (operation < 5) 
			return two_operand(operation);
		return one_operand(operation);
	}
	return zero_operand(operation);
}

 /**********************************************************************************************
 This function adds the data to data table when it is a line with data or string definition
 * @param type_val- the type of the definition
 * @return -1 if the is an error in the line or 0 otherwise
  ********************************************************************************************/
int add_to_data_table(int type_val) {
	char *tmp, *invalid=NULL;
	
	if (data_idx == data_len) { /*we reached the end of the line */
		p_errors("Missing value");
		return -1;
	}
	/*handle data type*/
	if (type_val == DATA) {
		while (data_idx < data_len) {
			strtol(data_arr[data_idx], &invalid, 10);
			if (!invalid[0]) { /*there's no invalis part*/
				if (!is_valid_val(data_arr[data_idx])) /*check if the number fits 21 bits*/
					p_errors("Number underflow or overflow");
			}
			else {
				if (invalid[0] != 13) { /*if there's an invalid part, the operand isn't an integer */
					p_errors("Number is not inteager");
					return -1;
				}
			}
			data_table[DC] = (word *)calloc(1, sizeof(word)); /*allocating memory for each integer */
			if (!data_table[DC]) {
				p_errors("Memory allocation failed");
				return -1;
			}
			data_table[DC]->val = atoi(data_arr[data_idx]);	 /*save the value*/
			DC++;
			data_idx += 2; /*every other operand is a comma */		
		}
		data_idx -= 2;/*in order to check for extra text */
	}
	else if (type_val == STRING) {
		if (*data_arr[data_idx] != '\"') { /*the first character after the definition has to be " */
			p_errors("Invalid code before string- there is no apostrophes");
			return -1;
		}		
		if (((with_label) && (data_len>3)) || ((!with_label) && (data_len > 2))){	/*check if we have more characters in the line  then needed*/
				p_errors("Invalid characters");
				return -1;
			
		}
		tmp = data_arr[data_idx];
		tmp++;
		while (*(tmp + 1)) /*scanning the characters in the string */
		{
			data_table[DC] = (word *)calloc(1, sizeof(word)); /*allocating memory for each character and inserting the current one */
			if (!data_table[DC]) {
				p_errors("Memory allocation failed");
				return -1;
			}
			data_table[DC++]->val = *(tmp++); 
		}
		data_table[DC] = (word *)calloc(1, sizeof(word)); 	/*allocating memory the last ("\0") character and inserting end of string */
		if (!data_table[DC]) {
			p_errors("Memory allocation failed");
			return -1;
		}
		data_table[DC++]->val = 0; 
	}
	return extra_text(); /*check for extra text*/
}
/**********************************************************************************************
This function search for the defintion of the line
* @return the definition value if the function found it or -1 otherwise
 ********************************************************************************************/
int get_def() {
	int i;
	remove_white_spaces(data_arr[data_idx]);
	for (i = 0; i < DEFINITION; i++) {
		if (!strcmp(definitions[i].op_name, data_arr[data_idx]))  /*if the string matches one of the definition names, we return the defintion's number */
			return i;
		
	}
	return -1;
}

/**********************************************************************************************
This function search for the operation of the line
* @return the operation if the function found it or -1 otherwise
 ********************************************************************************************/
int get_op() {
	int i,op;
	remove_white_spaces(data_arr[data_idx]);
	for (i = 0; i < OPERATIONS; i++) {
		if (strcmp(operations[i].op_name, data_arr[data_idx]) == 0) {  /*if the string matches one of the operation names, we return the operation's number */
			op_idx = data_idx;
			op= operations[i].op_val;
			return op;
		}
	}	
	return -1;
}

 /**********************************************************************************************
 This function check if ths value is valid by checking the range if the value
 * @param str- the string which contains the value
 * @return 2 if the value has up to 21 bits, 1 if the value between 0 and 24 and 0 otherwise
  ********************************************************************************************/
int is_valid_val(char *str) {
	
	int num, num_bits, val;
	num = atoi(str); /*convert the number from string to int*/
	if ((num == -1) || (num == 0))
		num_bits = 2;
	else { /*calculating the number of bits the number takes*/
		if (num > 0)
			num_bits = (int)(log(num) / log(2)) + 2;
		else {
			num_bits = (int)(log((-1)*num - 1) / log(2)) + 2;
		}
	}
	if (num_bits <= 21) {
		val = 2;
	}
	else
		val = 1;
	if ((num_bits >= 0) && (num_bits <= 24)) 
		return val;
	return 0;
}