#include "config.h"
#include "assembler.h"
#include "labels.h"
#include "word.h"
#include "def_op.h"
#include "commands.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**********************************************************************************************
This function check if the label is legal (there is ":" in the end of the label)
* @param str- the label that the function will check
* @return 1 if there is ":" in  the label and otherwise return 0
********************************************************************************************/
int legal_label(char *str) {
	
	int len = (int)strlen(str);
	int check = 0;
	if (len > MAX_LABEL_LEN) {
		return - 1; /*if the len of the label is bigger than MAX_LABEL_LEN, it is illegal*/
	}
	if (str[len - 1] == ':') /*if there is ':' int the end of the string, it is label*/
		check = 1;
	else
		check = 0;
	return check;
}

 /**********************************************************************************************
If a label exist, extract the label from the data_arr and copy it to the string
 * @param str- the string will copy to this variable in if exists
 * @param in_second_pass- a flag which indicates id we are in the second pass
 * @return 0 if the copy success or -1 if label is invalid
  ********************************************************************************************/
int extract_insert_label(char *str, int in_second_pass) {
	int len = (int)strlen(data_arr[data_idx]);
	
	/* if there is an operand and the last character is ':' or it is the second pass copy the data */
	if ((((data_arr[data_idx][len - 1]) == ':') && (data_arr[data_idx])) || (in_second_pass == 1)) {
		if (in_second_pass == 0) { /*in case it is the first pass change ':' to '\0'*/
			data_arr[data_idx][len - 1] = '\0';
			with_label = 1; /*a flag which indicet there is a label in the line*/
		}
		
		if (invalid_label(data_arr[data_idx])) /*if the label isn't valid we return an -1*/
			return -1;
		strcpy(str, data_arr[data_idx]); /*cppying the data to the label string */
		data_idx++;
		
	}
	else {
		if (isalpha(data_arr[data_idx][0]) && (!in_second_pass) && (get_def()==-1) && (get_op() == -1)) {
			with_label=-1;
		}
		*str = '\0'; /*if there's no label, we put end of string character at the beginning of the string */
	}
	
	
	return 0;
}

 /**********************************************************************************************
 This function checks if the label is valid or not
 * @param line- the label the function will check
 * @return 0 if the label is valid and -1 otherwise
  ********************************************************************************************/
int invalid_label(char *str) {
	
	int i;
	int res1 = 0, res2 = 0;
	int len = strlen(str);
	
	if (!isalpha(str[0])){ 
		if ((get_op() != -1) && (isdigit(str[0]))) { /*if it is not operation and the first char is a digit it is invalid operand*/
			p_errors("Invalid operand");
			return -1;
		}
		p_errors("lnvalid label"); /*if the first char is not letter it is invalid label*/
		return -1;
	}
	for (i = 1; i < len-1; i++) {/*scanning the string */
		if ((!isalpha(str[i])) && !(isdigit(str[i]))) {  /*if the second char is not letter and not digit it is invalid label*/
			p_errors("Invalid label");
				return -1;
		}	
	}
	if (strlen(str) == 2 && str[0] == 'r' && str[1] >= '0' && str[1] < '8'){ /*if the label is an operand name it is invalid label*/
			 p_errors("Invalid label");
			 return -1;
	}
			
	res1=check_labels(str,operations, OPERATIONS); /*The label cannot be an operation*/
	res2=check_labels(str,definitions, DEFINITION); /*The label cannot be a definition*/
	
	if ((res1 == -1) || (res2 == -1)) {
		return -1;
	}
	return 0;
}

 /**********************************************************************************************
 This function adds a label to symbol table
 * @param str- the label 
 * @param type- the type of the label
 * @param val- the value of the symbol in decimal. It will be the address of the symbol
 * @return -1 if there is another definition of the label and otherwise return 0
  ********************************************************************************************/
int add_label(char *str, int type, int val) {

	sym p_sym, tail_sym, node=NULL;
	int str_len= strlen(str);
	
	node = (sym)malloc(sizeof(symbols)); /*allocating memory for the new symbol */
	if (node == NULL) {
		p_errors("Memory allocation failed");
		return -1;
	}
	node->symbol_name = (char*)malloc(str_len+ 1); /*allocating memory for the symbol's name */
	if (node->symbol_name == NULL) {
		p_errors("Memory allocation failed");
		return -1;
	}
	strcpy(node->symbol_name, str); /*copying the symbols attributes to the struct*/
	node->symbol_type = type;
	node->symbol_val = val;
	node->next_symbol = NULL; /*the new node is now the tail of the symbol_table */
	
	if (head_sym_table == NULL) { /*If the symbol table is empty, the new node will be the head of the symbol_table*/
		head_sym_table = node;
		return 0;
	}

	p_sym = head_sym_table;
	while (p_sym!=NULL) { /*if the symbol tabel is not empty we scan the list until we reach it's tail */
		if (!strcmp(node->symbol_name, p_sym->symbol_name)) {/*if we have another definition of the same symbol, we have an error*/
			if ((p_sym->symbol_type == EXTERN) && (node->symbol_type == EXTERN)) {
				free(node->symbol_name);
				free(node);
				return 0;
			}
			p_errors("More than one definition of the same label");
			return -1;
		}
		if (!(p_sym->next_symbol))
			tail_sym = p_sym;
		p_sym = p_sym->next_symbol;
		
	}
	tail_sym->next_symbol = node; /*the new tail will be the new symbol */
	
	return 0;
}

 /**********************************************************************************************
 This function adds (when needed) entry to the typpe of the symbol
 * @param str- the symbol to which we will add the enty type
 * @return -1 if the symbol isn't defined or 0 otherwise
  ********************************************************************************************/
int add_entry_to_symbol(char *str) {
	
	sym p_sym = head_sym_table;
	remove_white_spaces(str); /*remove white spaces from the string*/
	while (p_sym!=NULL) { /*scanning the symbol table */	
		if (!strcmp(str, p_sym->symbol_name)) { /*if we found the symbol, we change its type to ENTRY*/
			p_sym->symbol_type = ENTRY;
			return 0;
		}
		p_sym = p_sym->next_symbol;
		
	}
	/*if we haven't found the symbol, the symbol isn't defined */
	p_errors("Symbol name isn't defined");
	return -1;
}

 /**********************************************************************************************
 This function gets the address of wanted symbol
 * @param str- the symbol we wand his address
 * @return the address of the symbol
  ********************************************************************************************/
int get_address_of_symbol(char *str) {
	
	int tmp_val, tmp_are;
	sym p_sym = head_sym_table;
	remove_white_spaces(str); /*remove white spaces from the string*/
	
	while (p_sym!=NULL) { /*scanning the symbol table */
		
		
		remove_white_spaces(p_sym->symbol_name);
		/*if we found the symbol, we insert its address to the relevent field in the instruction table*/
		if (!strcmp(str, p_sym->symbol_name)) {
			
			if (p_sym->symbol_type == EXTERN) {
				tmp_val = 0;
				tmp_are = E;
			}
			else {
				tmp_val = p_sym->symbol_val;
				tmp_are = R;
			}
			/*set the extra word*/
			set_extra_word(instruction_table[IC], tmp_val);
			set_are(instruction_table[IC],tmp_are); 
			
			IC++;	
			return p_sym->symbol_val;
		}
		p_sym = p_sym->next_symbol;
	}
	/* If we haven't found the symbol, the symbol isn't defined */
	p_errors("Symbol name isn't defined");
	return -1;
}

 /**********************************************************************************************
 This function adds IC to the address of each symbol with type DATA
  ********************************************************************************************/
void incr_address() {
	
	sym p_sym = head_sym_table;
	while (p_sym!=NULL) { /*scanning the symbol_table */
		if (p_sym->symbol_type == DATA) {/*if the symbol's type is data, we add its address by IC  */
			p_sym->symbol_val += IC; 
		}
		p_sym = p_sym->next_symbol;
	}
}

/**********************************************************************************************
This function checks if the label is valid (not operation and not a definition)
* @param str- the string which will checked
* @param arr- the array in which the function search for the label
* @param size- the length of the array
* @return -1 in case the string is an operation or definition (invalid label) and 0 otherwise
 ********************************************************************************************/
int check_labels(char *str,op *arr, int size) {
	int i;
	
	for (i = 0; i < size; i++) {
		if (!strcmp(str, arr[i].op_name)) {  /*label can't be an operation/definition name (depends on the array)*/
			p_errors("Invalid label");
			return -1;
		}
	}
	return 0;
}

 /**********************************************************************************************
 This function adds the address of the symbol to the matching place in the symbol tabel
 * @return -1 if the symbol wasn't defimed and 0 in case the adding succed
  ********************************************************************************************/
int add_address_to_symblos() {
	
	int address_method,dest_data_idx,dest_add;
	int address, op = get_op(); /*gets the operation */
	
	data_idx++; /*icrementing the data_index in order to get the first operand */
	
	if (op < 14){ /*if the operation number is less then 14, there's at least one operand */
		address_method = addressing_methods(data_arr[data_idx]);
		if (op >= 5) { /*if the operation number is more than 5, there's one operand*/
			if (address_method == DIRECT_REGISTER) {
				IC++;
				return 0;
			}
			if (address_method == DIRECT) {
				IC++;
				if ((address = get_address_of_symbol(data_arr[data_idx])) < 0) /*the symbol isn't defined */
					return -1;			
				if (!address) /*if the address is 0 - the symbol is extern, we'll add the appearance to the extern arr structure */
					add_to_extern_arr(data_arr[data_idx], IC-1);		
			}
			if ((op == JM_B_JS) && (address_method == RELATIVE)) { /*in case of relative method, calculate the distance and set the extra word*/
				IC++;
				add_relative_addressing(data_arr[data_idx]);
			}
			if (address_method == IMMEDIATE) { 
				IC+=2;
			}
		}		
		if (op < 5) {	/*if the operation number is less than 5, there is 2 operands*/
			dest_data_idx = data_idx + 2;
			dest_add = addressing_methods(data_arr[dest_data_idx]); /*find the dest addressing method*/
			
			/*handel the case of src addressing method is direct register and dest addressing methos is direct register*/
			if ((address_method == DIRECT_REGISTER) && (dest_add == DIRECT_REGISTER)) {				
				IC++;
				return 0;
			}
			/*handel the case of src addressing method is direct register and dest addressing methos is direct*/
			if ((address_method == DIRECT_REGISTER) && (dest_add == DIRECT)) {			
				IC++;
				address = get_address_of_symbol(data_arr[dest_data_idx]);
				if (address < 0) /*the symbol isn't defined */
					return -1;
				
				if (!address) /*if the address is 0 - the symbol is extern, we'll add the appearance to the extern arr structure */
					add_to_extern_arr(data_arr[dest_data_idx], IC-1);
			}
			/*handel the case of src addressing method is direct and dest addressing methos is direct*/
			if ((address_method == DIRECT) && (dest_add == DIRECT)) {				
				IC++;
				address = get_address_of_symbol(data_arr[data_idx]);
				if (address < 0) /*the symbol isn't defined */
					return -1;
				
				if (!address) /*if the address is 0 - the symbol is extern, we'll add the appearance to the extern arr structure */
					add_to_extern_arr(data_arr[data_idx], IC-1);
				address = get_address_of_symbol(data_arr[dest_data_idx]);
				if (address < 0) /*the symbol isn't defined */
					return -1;
				
				if (!address) /* If the address is 0 - the symbol is extern, we'll add the appearance to the extern_appearances structure */
					add_to_extern_arr(data_arr[dest_data_idx], IC-1);
			}
			/*handel the case of src addressing method is direct and dest addressing methos is direct register*/
			if ((address_method == DIRECT) && (dest_add == DIRECT_REGISTER)) {		
				IC++;
				address = get_address_of_symbol(data_arr[data_idx]);
				if (address < 0) /* The symbol isn't defined */
					return -1;
				
				if (!address) /* If the address is 0 - the symbol is extern, we'll add the appearance to the extern_appearances structure */
					add_to_extern_arr(data_arr[data_idx], IC-1);
			}	
			/*handel the case of src addressing method is direct and dest addressing methos is immediate*/
			if ((address_method == DIRECT) && (dest_add == IMMEDIATE)) {
				IC++;
				address = get_address_of_symbol(data_arr[data_idx]);
				if (address < 0) /* The symbol isn't defined */
					return -1;
				
				if (!address) /* If the address is 0 - the symbol is extern, we'll add the appearance to the extern_appearances structure */
					add_to_extern_arr(data_arr[data_idx], IC-1);
				IC++;
			}
		}	
	}

		return 0;
}

 /**********************************************************************************************
 In case of extern fuction, this function adds an aooearance to the extern array structure
 * @param str- the symbol's name
 * @param val- the value of the symbol (his address)
  ********************************************************************************************/
void add_to_extern_arr(char *str, int val) {
	if (!extern_arr) { /*allocating memory to extern arr if it doesn't exist*/
		extern_arr = (op **)calloc(1,sizeof(op *));
	}
	else
		extern_arr = (op **)realloc(extern_arr, (IC + 1) * sizeof(op *));  /*reallocating memory to extern arr*/
	
	if (!extern_arr) {
		p_errors("Memory allocation failed");
		
	}

	/* allocating memory for the current symbol*/
	extern_arr[extern_counter] = (op *)malloc(sizeof(op));
	if (!extern_arr[extern_counter]) {
		p_errors("Memory allocation failed");
		
	}
	/* allocating memory for the current symbol name*/
	extern_arr[extern_counter]->op_name = (char *)malloc(strlen(data_arr[data_idx]) + 1);
	if (!extern_arr[extern_counter]->op_name) {
		p_errors("Memory allocation failed");
		
	}
	strcpy(extern_arr[extern_counter]->op_name, str); /*copying the name of the symbol to the structure */
	if (!(extern_arr[extern_counter]->op_name))
		p_errors("Memory allocation failed");
	
	extern_arr[extern_counter++]->op_val = val; /*copying the val of the symbol to the structure */
}

/**********************************************************************************************
In case of relative addressing method, this calculate the distance between the symbols addersses
and set the extra word
* @param str- the symbol's name
 ********************************************************************************************/
void add_relative_addressing(char *str) {
	int distance,sym_address, my_address = IC;
	sym p_sym = head_sym_table;
	while (p_sym != NULL) {/*scanning the symbol_table */

		if (!strcmp(str, p_sym->symbol_name)) { /*if we found the symbol, save its value*/
			sym_address = p_sym->symbol_val;
		}
		p_sym = p_sym->next_symbol;
	}
	distance = sym_address-my_address+1; /*calculate the distanc between two addresses*/
	set_extra_word(instruction_table[IC], distance); /*set the extra word*/
	set_are(instruction_table[IC++], A);
			
}