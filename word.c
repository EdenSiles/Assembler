#include "config.h"
#include "assembler.h"
#include "word.h"

/**********************************************************************************************
This function set the value to the appropriate part in the word
* @param word- a pointer to the word we are going to change
* @param num- the value we are going to set in the word 
* @param fields_first_bit- the first position where we are going set the val
* @param fields_len- the length of the part we are going to set
 ********************************************************************************************/
void set_part_to_word(word *word, int num, int fields_first_bit, int fields_len) {
	/*using bitwise operations to change only the wanted bits*/
   word->val &= ~(((1<< fields_len)-1)<< fields_first_bit);
   word->val += num * (1 << fields_first_bit);
}
 /**********************************************************************************************
 This function set the opcode part in the word.
 * @param word- a pointer to the word we are going to change
 * @param num- the value of the opcode
  ********************************************************************************************/
void set_opcode(word *word, int num) {
	set_part_to_word(word, num, OPCODE_BIT, OPCODE_LEN);
}
/**********************************************************************************************
This function set the source addressing method part in the word.
* @param word- a pointer to the word we are going to change
* @param num- the value of the opcode
 ********************************************************************************************/
void set_src_address(word *word, int num) {
	set_part_to_word(word, num, SRC_ADD_BIT, SRC_ADD_LEN);
}
/**********************************************************************************************
This function set the source register number method part in the word. part in the word.
* @param word- a pointer to the word we are going to change
* @param num- the value of the opcode
 ********************************************************************************************/
void set_src_reg(word *word, int num) {
	set_part_to_word(word, num, SRC_REG, SRC_REG_LEN);
}
/**********************************************************************************************
This function set the destinition addressing method part in the word.
* @param word- a pointer to the word we are going to change
* @param num- the value of the opcode
 ********************************************************************************************/
void set_dest_address(word *word, int num) {
	set_part_to_word(word, num, DEST_ADD_BIT, DEST_ADD_LEN);
}
/**********************************************************************************************
This function set the destinition register number method part in the word. part in the word.
* @param word- a pointer to the word we are going to change
* @param num- the value of the opcode
 ********************************************************************************************/
void set_dest_reg(word *word, int num) {
	set_part_to_word(word, num, DEST_REG_BIT, DEST_REG_LEN);
}
/**********************************************************************************************
This function set the funct part in the word.
* @param word- a pointer to the word we are going to change
* @param num- the value of the opcode
 ********************************************************************************************/
void set_funct(word *word, int num) {
	set_part_to_word(word, num, FUNCT_BIT, FUNCT_LEN);
}
/**********************************************************************************************
This function set the ARE part in the word.
* @param word- a pointer to the word we are going to change
* @param num- the value of the opcode
 ********************************************************************************************/
void set_are(word *word, int num) {	
	set_part_to_word(word, num, ARE_BIT, ARE_LEN);
}
/**********************************************************************************************
This function set the value of the extra word (when we have another word)
* @param word- a pointer to the word we are going to change
* @param num- the value of the opcode
 ********************************************************************************************/
void set_extra_word(word *word,int num){	
		set_part_to_word(word,num, EXTRA_WORD_BIT, EXTRA_WORD_LEN);
}

