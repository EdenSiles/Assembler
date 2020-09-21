#include "config.h"
#include "assembler.h"

#define DATA_TABLE_SIZE 100 
#define INST_TABLE_SIZE 500

op definitions[] = { {".data", DATA,0}, {".string",STRING,0},{".entry", ENTRY,0},{".extern", EXTERN,0} ,{".code", CODE,0} };
op operations[] = { {"mov",MOV,0},{"cmp",CMP,0} ,{"add",A_S,ADD},{"sub",A_S,SUB},{"lea",LEA,0},{"clr",C_N_I_D,CLR},{"not",C_N_I_D,NOT},{"inc",C_N_I_D,INC},
					{"dec",C_N_I_D,DEC},{"jmp",JM_B_JS,JMP},{"bne",JM_B_JS,BNE},{"jsr",JM_B_JS,JSR},{"red",RED,0},{"prn",PRN,0},{"rts",RTS,0},{"stop",STOP,0} };

word *instruction_table[INST_TABLE_SIZE];
word *data_table[DATA_TABLE_SIZE]; 


 /**********************************************************************************************
 This function fress the memory allocation of the global structures
  ********************************************************************************************/
void frees_memory() {
	int i;
	sym p_sym = head_sym_table, next_sym;
	for (i = 0; i < DC; i++) { /*frees the data_table */
		free(data_table[i]);
	}
	for (i = 0; i < IC; i++) { /*frees the instruction_table */
		free(instruction_table[i]);
	}
	for(i = 0; i < extern_counter; i++) /*frees the extern arr_table and the structure itself */
	{
		free(extern_arr[i]->op_name);
		free(extern_arr[i]);
	}
	free(extern_arr); 

	for (i = 0; i < data_len; i++) /*frees the data_arr*/
		free(data_arr[i]);
	free(data_arr);

	while (p_sym) { /*frees the symbol_table*/
		next_sym = p_sym->next_symbol;
		free(p_sym);
		p_sym = next_sym;
	}
}

/**********************************************************************************************
This function removes all the white spaces from the given string
* @param str- the string from which the function remove all the white spaces
 ********************************************************************************************/
void remove_white_spaces(char *str) {
	char *trim = NULL;
	int i, len, start;
	if (str != NULL) { /*if the string exist*/
		i = 0;
		len = strlen(str);
		
		trim = (char *)malloc(len); /*allocate memory for the temporary string*/
		start = get_first_position(str); /*find the first position which isn't white space*/
		while (i < len) { /*scaning the string*/
			if (str[start] == '\r') { 
				break;
			}
			trim[i] = str[start]; /*copy chars to the temporary string*/
			i += 1;
			start += 1;
		}
		trim[i] = '\0';	/*end the string*/
	}
	strcpy(str, trim); /*copy the temporary string to our string*/
	free(trim); /*frees the temporary string*/
}

/**********************************************************************************************
This function is an auxiliary function to skip white spaces by checking char
* @param c- the char which the function will check if it is a white space
 ********************************************************************************************/
int is_white_space(char c) {
	return (c == ' ' || c == '\t' || c == '\n');
}

/**********************************************************************************************
This function is an auxiliary function which iterate through the white spaces at the beginning 
of the string
* @param str- the string which will be checked
* @return the index of the first position which is not white space
 ********************************************************************************************/
int get_first_position(char *str) {
	int i = 0;
	while (is_white_space(str[i])) {
		i += 1;
	}
	return i;
}

/**********************************************************************************************
This function is an auxiliary function which iterate through the white spaces at the end
of the string
* @param str- the string which will be checked
* @return the index of the last position which is not white space
 ********************************************************************************************/
int get_last_position(char *str) {
	int i = strlen(str) - 1;
	while (is_white_space(str[i])) {
		i -= 1;
	}
	return i;
}

/**********************************************************************************************
This function calculate the length of the string without the white spaces
* @param str- the string which will be checked
* @return the correct length of a trimmed string
 ********************************************************************************************/
int get_trim_len(char *str) {
	return (get_last_position(str) - get_first_position(str));
}