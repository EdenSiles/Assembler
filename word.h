#ifndef WORD_H
#define WORD_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**********************************************************************************************
This function set the value to the appropriate part in the word
* @param word- a pointer to the word we are going to change
* @param num- the value we are going to set in the word
* @param fields_first_bit- the first position where we are going set the val
* @param fields_len- the length of the part we are going to set
 ********************************************************************************************/
void set_part_to_word(word *word, int num, int fields_first_bit, int fields_len);
/**********************************************************************************************
 This function set the opcode part in the word.
 * @param word- a pointer to the word we are going to change
 * @param num- the value of the opcode
  ********************************************************************************************/
void set_opcode(word *word, int num);
/**********************************************************************************************
This function set the source addressing method part in the word.
* @param word- a pointer to the word we are going to change
* @param num- the value of the opcode
 ********************************************************************************************/
void set_src_address(word *word, int num);
/**********************************************************************************************
This function set the source register number method part in the word. part in the word.
* @param word- a pointer to the word we are going to change
* @param num- the value of the opcode
 ********************************************************************************************/
void set_src_reg(word *word, int num);
/**********************************************************************************************
This function set the destinition addressing method part in the word.
* @param word- a pointer to the word we are going to change
* @param num- the value of the opcode
 ********************************************************************************************/
void set_dest_address(word *word, int num);
/**********************************************************************************************
This function set the destinition register number method part in the word. part in the word.
* @param word- a pointer to the word we are going to change
* @param num- the value of the opcode
 ********************************************************************************************/
void set_dest_reg(word *word, int num);
/**********************************************************************************************
This function set the funct part in the word.
* @param word- a pointer to the word we are going to change
* @param num- the value of the opcode
 ********************************************************************************************/
void set_funct(word *word, int num);
/**********************************************************************************************
This function set the ARE part in the word.
* @param word- a pointer to the word we are going to change
* @param num- the value of the opcode
 ********************************************************************************************/
void set_are(word *word, int num);
/**********************************************************************************************
This function set the value of the extra word (when we have another word)
* @param word- a pointer to the word we are going to change
* @param num- the value of the opcode
 ********************************************************************************************/
void set_extra_word(word *word, int num);

#endif