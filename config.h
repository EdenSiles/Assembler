#ifndef CONFIG_H
#define CONFIG_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum type_def_val{DATA,STRING,ENTRY,EXTERN,CODE};
enum addressing_val {IMMEDIATE,DIRECT,RELATIVE,DIRECT_REGISTER};
enum are {A=4,R=2,E=1};
enum fields_len { OPCODE_LEN = 6, SRC_ADD_LEN = 2, SRC_REG_LEN = 3, DEST_ADD_LEN = 2, DEST_REG_LEN = 3, FUNCT_LEN = 5, ARE_LEN = 3,EXTRA_WORD_LEN=21 };
enum fields_first_bit { OPCODE_BIT = 18, SRC_ADD_BIT = 16, SRC_REG = 13, DEST_ADD_BIT = 11, DEST_REG_BIT = 8, FUNCT_BIT = 3, ARE_BIT = 0,EXTRA_WORD_BIT=3 };
enum op{MOV,CMP,A_S,LEA=4,C_N_I_D=5,JM_B_JS=9,RED=12,PRN=13,RTS=14,STOP=15};
enum A_S_op{ADD=1,SUB=2};
enum C_N_I_D_op{CLR=1,NOT=2,INC=3,DEC=4};
enum JM_B_JS_op{JMP=1,BNE=2,JSR=3};


typedef struct symbols *sym;
typedef struct symbols {/* represents a symbol in a linked list */
	char *symbol_name; /*the name of the symbol */
	int symbol_val; /*the index in the data table in which the symbol data is held, or 0 if it's an external variable */
	int symbol_type; /*the type of the symbol*/
	sym next_symbol; /*a pointer to the next symbol in the list*/
}symbols;
sym head_sym_table;

/*data structures which represent definitions and operations*/
typedef struct {
	char *op_name; /*the name of the operation or definition*/
	int op_val; /*the value of the operation or definition*/
	int funct; /*the funct of the operation or  0 if it is definition*/
}op;

extern op **extern_arr; /*holds the external variables in the machine code */
extern op operations[]; /*holds each operation name, his funct and the number that matches it */
extern op  definitions[]; /*holds each definition name and the number that matches it */

typedef struct { /* This struct represents a word in the assembly code */
	unsigned int val : 24;
}word;

extern word *instruction_table[];
extern word *data_table[];

#endif