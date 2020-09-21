#include "assembler.h"
#include "config.h"
#include "files.h"
#include <string.h>

/**********************************************************************************************
This function opens an assembly file and calls the functions that make the first and second pass.
In case there were no errors, the function creates the output files. 
 * @param argc - the amount of assembly files
 * @param argv - the assembly files
  ********************************************************************************************/
void open_files(int argc, char *argv[])
{
    FILE *fp;
    int i;
    int file_name_len = 0,total_len=0;
    int final_addition_len = strlen(".as");
	    
    for (i=1; i < argc; i++) /* scanning argv from the second argument which is the first file name to be checked */
    {
        /* assigns memory for the file's name copies it, including the ".as" ending */
        file_name_len = strlen(argv[i]);
        total_len = file_name_len + final_addition_len;
        file_name = (char *)malloc(total_len);
        if (!file_name) {
            p_errors("memory allocation failed");         
        }
        strcpy(file_name, argv[i]);
        strcat(file_name, ".as");

        fp = fopen(file_name, "r");    /* opens the file for reading, if it exists */
        if (!fp){  /* if the wasn't found, or it isn't allowed for reading, the file pointer is NULL */
            fprintf(stderr, "ERROR: couldn't open the file %s\n", file_name);
            continue;
        }

        /* executes the first and second passes*/ 
        if (!first_pass(fp) && !second_pass(fp)){
			create_output_files(argv[i]); /*if the passes are finished successfully, writes the output files */
		}
        fclose(fp); /*closes the file after reading and frees the file_name string for the next file name */      
        free(file_name);   
    }
}

 /**********************************************************************************************
 This function creates a file with write permitions. 
 In case there were no errors, the function creates the output files.
 Input parameters:
 * @param - the name of the new file
 * @param- the end of the file (".ob"/".ent"/".ext")
 * @return a pointer to the new file
  ********************************************************************************************/
FILE *create_file(char *str, char *end) {
   
    FILE *fp;
    
    int str_len = strlen(str);
    int end_len = strlen(end);
    char *file_name = (char *)malloc(str_len + end_len); /* Allocates memory for the file's name*/
    if (file_name == NULL) {
        p_errors("memory allocation failed");
        return NULL;
    }
	/*copies the string and the ending into the file name */
    strcpy(file_name, str);
    strcat(file_name, end);
    fp = fopen(file_name, "w"); /*Opens the file, with writing permission */
    free(file_name); /*The file's name isn't needed anymore */
    return fp;
}

 /**********************************************************************************************
 This function create a file with ".ext" ending and write in this file the extern labels 
 and their addresses.
 Input parameters:
	 # str - the file name (without the ".as" ending)
 Returns:
	 # nothing - void function
  ********************************************************************************************/
void create_ext_file(char *str) {
    FILE *fp;
    int i;

    if (extern_counter == 0)
        return;
    fp = create_file(str, ".ext"); /*create a file with ".ext" ending and writing permission */
    for (i = 0; i < extern_counter; i++) { /*scanning the extern_arr table and printing each appearance to the file */
        fprintf(fp, "%s\t%07d\n", extern_arr[i]->op_name, extern_arr[i]->op_val);
    }
    fclose(fp); /*frees the file pointer after use */
}


 /**********************************************************************************************
 This function create a file with ".ob" ending and write in this file the instruction and data
 words and their addresses.
 * @param str - the file name (without the ".as" ending)
  ********************************************************************************************/
void create_ob_file(char *str) {

    FILE *fp;
    int i;

    fp = create_file(str, ".ob"); /*create a file with ".ob" ending and writing permission */
    fprintf(fp, "\t%d %d\n", ICF-100, DCF); /*prints the number of instruction and data words */
	
	for (i = FIRST_ADDRESS; i < ICF; i++) { /*prints each instruction word */
		fprintf(fp, "%07d %06x\n", i, instruction_table[i]->val);
	}
	for (i = 0; i < DCF; i++) {  /*prints each data word */
		fprintf(fp, "%07d %06x\n", i + ICF, data_table[i]->val);
	}
    fclose(fp);
}

/**********************************************************************************************
This function create a file with ".ent" ending and write in this file the entry labels
and their addresses.
 * @param str - the file name (without the ".as" ending)
 ********************************************************************************************/
void create_ent_file(char *str) {

    FILE *fp=NULL;
    sym p_sym = head_sym_table;
    int first_entry = 1;

    while (p_sym!=NULL) { /*scanning the symbol_table */
        if (p_sym->symbol_type == ENTRY) { /*if the symbol type is entry*/
            if (first_entry == 1) { /*we have a first entry variable */
                first_entry = 0; /*don't enter this condition again */
                fp = create_file(str, ".ent");	/*create a file with ".ent" ending and writing permission */
            }
            fprintf(fp, "%s %07d\n", p_sym->symbol_name, p_sym->symbol_val); /*prints every entry symbol and its address, to the file*/
        }
        p_sym = p_sym->next_symbol;
    }
    fclose(fp); /*frees the file pointer after use */
}

 /**********************************************************************************************
 This function creates all the output files 
 * @param str - the file name (without endings)
  ********************************************************************************************/
void create_output_files(char *str){
	create_ext_file(str); /*create ".ext" ourput file*/
    create_ent_file(str); /*create ".ent" ourput file*/
	create_ob_file(str); /*create ".ob" ourput file*/
    
}