#include "config.h"
#include "assembler.h"
#include "commands.h"
#include "word.h"
#include "labels.h"
#include "def_op.h"
#include <ctype.h>

op **extern_arr;

/**********************************************************************************************
This function creates an assembly code for an operation with 0 operands
* @param op- the value of the opertaion
* @return 0 if the creating succeed and -1 if there werw an error
 ********************************************************************************************/
int zero_operand(int op) {
	if (extra_text() == -1) /*checking for extra text after the operation, which is invalid */
        return -1;

	instruction_table[IC] = (word*)calloc(1,sizeof(word));/*allocating memory for the machine code */
	if (instruction_table[IC] == NULL)
        p_errors("Memory allocation failed");

	set_opcode(instruction_table[IC], op);/*set the word*/ 
	set_are(instruction_table[IC], A); /*A in the are when it the first word*/
	IC++;

    return 0;
}

/**********************************************************************************************
This function creates an assembly code for an operation with 1 operands
* @param op- the value of the opertaion
* @return 0 if the creating succeed and -1 if there werw an error
 ********************************************************************************************/
int one_operand(int op) {
	
    int op_dest_val,dest_reg;
	int addressing;
	if (data_arr[data_idx] == NULL) {
		p_errors("Missing operand");
		return -1;
	}
	if (data_arr[data_idx+1] != NULL){
		p_errors("Invalid operand");
		return -1;
	}
	addressing= addressing_methods(data_arr[data_idx]); /*find the addressing method*/
    if (addressing == -1) { /*the addressing method is invalid */
        return -1;
    }
	
	if (extra_text()) { /*checking for extra text after the operand, which is invalid */
		return -1;
	}
	
	op_dest_val = get_op_res(addressing, data_idx); /*get the destination value*/
	
    if (op != PRN) { /*check for illegal addressing method*/
        if (((op == JM_B_JS) && (addressing == DIRECT_REGISTER)) || (addressing == IMMEDIATE) || ((op != JM_B_JS) && (addressing == RELATIVE))) {
            p_errors("Invalid addressing method for destination operand");
            return -1;
        }
    }
	if (addressing != DIRECT_REGISTER) /*id addressing method isn't direct register, dest reg field in the word is 0*/
		dest_reg = 0;
	else
		dest_reg = op_dest_val;

    instruction_table[IC] = (word*)calloc(1,sizeof(word)); /*allocating memory for the machine code */
	if (instruction_table[IC] == NULL) {
		p_errors("Memory allocation failed");
	}
	if (addressing != DIRECT_REGISTER) {
		instruction_table[IC + 1] = (word*)calloc(1, sizeof(word));/*allocating memory for the next word in the machine code */
		if  (instruction_table[IC + 1] == NULL)
			p_errors("Memory allocation failed");
	}
    set_opcode(instruction_table[IC], op); /*set the first word of the operation. A is the are in this case*/
	set_funct(instruction_table[IC], get_funct());
    set_dest_address(instruction_table[IC], addressing);
    set_dest_reg(instruction_table[IC], dest_reg);
    set_are(instruction_table[IC], A);
	IC++;
	
    if (((op == JM_B_JS) && (addressing == RELATIVE))||(addressing == DIRECT)) { /*in this case we have extra word and we set the word in second pass*/
		IC++;
		return 0;
    }

	else if ((addressing != DIRECT_REGISTER) && (addressing != DIRECT)) { /*set the extra word. A is the are*/
       set_are(instruction_table[IC], A);
       set_extra_word(instruction_table[IC], op_dest_val);
	   IC++;
	   return 0;
    }
	return 0; 
}

 /**********************************************************************************************
 This function creates an assembly code for an operation with 2 operands
 * @param op- the value of the opertaion
 * @return 0 if the creating succeed and -1 if there werw an error
  ********************************************************************************************/
int two_operand(int op) {
    int addressing_src, addressing_dest, src_is_reg=0, dest_is_reg=0, op_src, op_dest, src_reg=0, dest_reg=0;
	int funct = get_funct(); /*get funct of the operation*/
	
	if ((data_arr[data_idx] == NULL) || (data_arr[data_idx + 1] == NULL) || (data_arr[data_idx + 2] == NULL)) {
		p_errors("Missing operand");
		return -1;
	}
    
	addressing_src = addressing_methods(data_arr[data_idx]); /*find addressing method of src*/
	addressing_dest = addressing_methods(data_arr[data_idx + 2]); /*find addressing method of src*/
	
	if ((addressing_src==RELATIVE)|| (addressing_dest == RELATIVE)) { /*addressing method can't be relative*/
		p_errors("Invalid addressing method for source or destination operand");
		return -1;
	}
	if ((addressing_src == -1) || (addressing_dest == -1)) {
		return -1;
	}
	op_src = get_op_res(addressing_src, data_idx); /*get sorce operand value*/
	op_dest = get_op_res(addressing_dest, data_idx + 2); /*get destination operand value*/
		
	if (addressing_src == DIRECT_REGISTER) { /*check if src is register*/
		src_is_reg = 1;
		src_reg = op_src;
	}
	if (addressing_dest == DIRECT_REGISTER) { /*check if dest is register*/
		dest_is_reg = 1;
		dest_reg = op_dest;
	}
	
    data_idx += 3;
    if (extra_text()) /*check if there is extra tetx in the end of the command*/
        return -1;

    if (((op == LEA) && (addressing_src != DIRECT)) || ((op != CMP) && addressing_dest == IMMEDIATE)){
         p_errors("Invalid addressing method for source or destination operand");
         return -1;
    }
	    
    instruction_table[IC] = (word*)calloc(1,sizeof(word)); /*allocating memory for the machine code */
	
	if (instruction_table[IC] == NULL) {
		p_errors("Memory allocation failed");
	}
   
	if ((src_is_reg == 0) || (dest_is_reg == 0)) { /*if at least one of the operant isn't register, we have another word*/
		instruction_table[IC + 1] = (word*)calloc(1, sizeof(word)); /*allocating memory for extra word*/
		if  (instruction_table[IC + 1] == NULL)
            p_errors("Memory allocation failed");
    }
   	
    if ((src_is_reg == 0) && (dest_is_reg == 0)) {/*src and dest is not registers, wh have third word*/
		instruction_table[IC + 2] = (word*)calloc(1, sizeof(word));
		if (instruction_table[IC + 2] == NULL)
            p_errors("Memory allocation failed");
    }
	
        set_opcode(instruction_table[IC], op); /*set the first word. are ia A*/
		set_funct(instruction_table[IC], funct);
		set_dest_address(instruction_table[IC], addressing_dest);
		set_dest_reg(instruction_table[IC], dest_reg);
		set_src_address(instruction_table[IC], addressing_src);
		set_src_reg(instruction_table[IC], src_reg);
		set_are(instruction_table[IC], A);
		IC++;
		
		
		if (src_is_reg == 0) { /*if source isn't register, set the second word*/
			if (addressing_src == IMMEDIATE) {
				set_extra_word(instruction_table[IC], op_src);
				set_are(instruction_table[IC++], A);
			}
			else if (addressing_src == DIRECT) { /*direct addressing will handle in second pass*/
				IC++;			
			}
			
			if (dest_is_reg == 0) { /*if destination isn't register, set the thired word*/
				if (addressing_dest == IMMEDIATE) {
					set_extra_word(instruction_table[IC], op_dest);
					set_are(instruction_table[IC], A);
					IC++;		
				}
				else if (addressing_dest == DIRECT) {	/*direct addressing will handle in second pass*/				
					IC++;					
				}
			}
		}
		else { /*source is a register*/
			
			if (dest_is_reg == 0) {  /*if destination isn't register, set the second word*/
				if (addressing_dest == IMMEDIATE) {
					set_extra_word(instruction_table[IC], op_src);
					set_are(instruction_table[IC], A);
					IC++;
				}
				else if (addressing_dest == DIRECT) { /*direct addressing will handle in second pass*/
					IC++;	
				}
			}
		}    

        return 0;
}

/**********************************************************************************************
This function gets the addressing method of the operand by analizes the operand's string
* @param operand- the operand's string 
* @return the number of the addressing method or -1 in case there is an error
 ********************************************************************************************/
int addressing_methods(char* operand) {

    char* invalid;
    int res = 0, reg_num;
	
    if (!data_len) {
        p_errors("Missing operand");
        return -1;
    }
    if (*operand == '&') { /*if the first character is '&' check if it relative method*/
		res=strtol(operand+1, &invalid, 10); /*invalid holds the invalid part */
		if ((res != 0) || (!isalpha(invalid[0]))) { /*if the second character isn't letter, it is invalid label*/
			p_errors("Invalid label");
			return -1;
		}
		strcpy(operand, invalid);
		res = invalid_label(invalid);
		if (res != -1) {
			return RELATIVE;
		}
		else {
			return -1;
		}
    }
  
    reg_num = get_reg_num(operand, &invalid);
	
	if (operand[0] == 'r' && (reg_num >= 0) && (reg_num <= 7)) { /*check if the operand is register*/
		return DIRECT_REGISTER;
	}
	if (*operand == '#') { /*if the first character is '#', a valid operand will be a number */

		remove_white_spaces(operand);
        res=strtol(operand + 1, &invalid, 10); /*invalid holds the invalid part */

        if (!invalid[0]) { /*if there's no invalid part, we check if the number fits 21 bits, including MSB as sign flag */
			if (is_valid_val(operand + 1) == 2)
                return IMMEDIATE;
            else {
                p_errors("Number overflow or underflow");
                return -1;
            }
        }
        p_errors("Not an integer"); /*if there's an invalid part, the operand isn't an integer */
        return -1;

    }
    if (invalid_label(operand)) {
        return -1;
    }
	else {
		return DIRECT;
	}
}

/**********************************************************************************************
According to the operand's addressing method, this function calculates the operand value
* @param addressing- the addressing method of the operand
* @param operand - the index of data in which the operand's name is contained
* @return the value of the operand 
 ********************************************************************************************/
int get_op_res(int addressing, int operand) {
	if (addressing == DIRECT) { /*there's no value for direct method*/
		return 0;
	}
	if ((addressing == DIRECT_REGISTER) || (addressing == IMMEDIATE)) { /*calculate the value of the operand*/
		return atoi(data_arr[operand] + 1);
	}
    return -1;
}

/**********************************************************************************************
This function search for an operation the matches the string and if it has found return its 
funct field.
* @return the funct of the operand if the string matches one of the operations and -1 otherwise
 ********************************************************************************************/
int get_funct() {
	int i;
	for (i = 0; i < OPERATIONS; i++) {		
		if (strcmp(operations[i].op_name, data_arr[op_idx]) == 0)  /*if the string matches one of the operation names, we return the operation's funct number */
			return operations[i].funct;
	}
	return -1;

}

/**********************************************************************************************
This function calculate the register number
* @param operand- the operand's string
* @param ptr- will contains the charecter in this string which isn't a number ("r")
* @return the register number
 ********************************************************************************************/
int get_reg_num(char* operand, char** ptr) {
    return strtol(operand, ptr, 10);
}

/**********************************************************************************************
This function checks in the symbol tabel if the given operand is a label
* @param operand- the operand's string
* @return 1 if the operand is a label and 0 otherwise
 ********************************************************************************************/
int check_if_label(char* operand) {
    sym p_sym = head_sym_table;
    while (p_sym != NULL) {
        if (strcmp(p_sym->symbol_name, operand) == 0) /*check if the operand is symbol*/
            return 1;
        p_sym = p_sym->next_symbol;
    }
    return 0;
}