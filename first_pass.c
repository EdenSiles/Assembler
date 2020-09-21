#include "config.h"
#include "assembler.h"
#include "check_line.h"
#include "labels.h"
#include "def_op.h"




 /**********************************************************************************************
 This function execute the first pass on the given file and search for syntax error
 * @param fp- a pointer to the file
 * @return 0 if the function finish the first pass in succes or 1 otherwise
  ********************************************************************************************/
int first_pass(FILE *fp) {

	char line[MAX_LINE_LEN], label[MAX_LABEL_LEN];
	int illegal = 0, data_error = 0;
	op_idx = 0;
	IC = 100;
	DC = 0;
	line_idx = 0;

	while (fgets(line, MAX_LINE_LEN, fp)) {/*scanning through each line of the file */
		remove_white_spaces(line);	/*remove white spaces from the line*/
		
		with_label = 0;
		line_idx++;
		data_idx = 0;

		data_len = check_line(line); /*getting the line arguments after analyzing*/	
		if (data_len <= 0) { /*if the line is empty, or there's an error, we continue to the next line */
			if(data_len < 0)
				illegal = 1; /*there is an error */
			continue;
		}
		if (illegal_commas() || missing_commas()) {/*there's an error with the commas in the line */
			illegal = 1;
			continue;
		}
		
		if (extract_insert_label(label,0)) {/*if the label exist, get it. If the label is invalid, there's an error*/
			illegal = 1;
			continue;
		}	
		if (data_idx == data_len) 
			continue;	

		data_error = insert_data(label);
		if (data_error == -1) {/*If there's a syntax error in a directive sentence, we continue to the next line */
			illegal = 1;
			continue;
		}			
		if (data_error && insert_instruction(label)) { /*insert instruction to instruction table*/
			illegal = 1;
		}
	}
	if (illegal == 0) { /*If there were no errors, increase the addresses of the data variables*/
		incr_address();
	}			
	ICF = IC; 
	DCF = DC;	

	rewind(fp); /*rewinds the file for the second pass and returns a flag regarding a spotted error in the file */
	return illegal;
}


