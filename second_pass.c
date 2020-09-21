#include "config.h"
#include "assembler.h"
#include "labels.h"
#include "def_op.h"
#include "check_line.h"


 /**********************************************************************************************
 This function execute the second pass on the given file and search for syntax error. Moreover,
 this function extracting labels data from the symbol tabel and inserting it to the machine code
 * @param fp- a pointer to the file
 * @return 0 if the function finish the second pass in succes or 1 otherwise
  ********************************************************************************************/
int second_pass(FILE *fp) {

    char line[MAX_LINE_LEN], label[MAX_ARGS_LEN];
    int error = 0, definition;
	IC = 100;
    extern_counter= 0;
    line_idx = 0;
   
	while (fgets(line, MAX_LINE_LEN, fp)) {	
        line_idx++;
        data_idx = 0;
        data_len = check_line(line); /*getting the line arguments after parsing */
        if (data_len == 0) /*if the line is empty, we continue to the next line */
            continue;		
        if (extract_insert_label(label, 0)) { /*if the label exist we get the label. If the label is invalid, there's an error */   
	        error = -1;
            continue;
        }

        definition = get_def();	 /*checks if there's any definition identified */
        if (definition != -1) {        
            if (definition != ENTRY) /* if the defintion is not entry- continue */
                continue;
            data_idx++; /*Incrementing data index to get the entry label */
          
            if (extract_insert_label(label, 1)) {   /*gets the label in the entry definition. If the label is invalid, there's an error */
                error = -1;                           
                continue;
            }			
			if (add_entry_to_symbol(label)) { /*If there isn't a symbol with the given name, there's an error */
				error = -1;
			}
        }
        else {         
			if (add_address_to_symblos()) {	/*add the address of each symbol*/
				error = -1;
			}
        }
    }
    return error; /*returns a flag regarding an error in the file */
}


