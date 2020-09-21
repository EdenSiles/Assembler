#include "config.h"
#include "assembler.h"
#include "check_line.h"

 /**********************************************************************************************
 This function check the given line and search for string, commas and white characters.
 * @param line- the assembly line
 * @return value is the number of opernd in the line or -1 in case illegal string argument
  ********************************************************************************************/
int check_line(char* line) {
	int i = 0;
	char *token, *end_ptr, str[MAX_LINE_LEN];
	
	data_arr = (char **)malloc(sizeof(char *)); /*allocate memory for the global data array*/
	if (data_arr == NULL)
		return -1;
	
	if (get_str(line, str, strchr(line, '\"'))) /*if there's an error int assembly strings, we return -1*/
		return -1;
		
	end_ptr = strrchr(line, '\"'); /*getting the string's closing quote, if exists */
	token = strtok(line, " \t\n"); /*getting the first argument, if exists */	
	while (token){
		if ((*token == '\"') && (*str)) {
			if (data_arr != NULL) {
				/*if we reached an assembly language string, we will allocate memory for it */
				data_arr[i] = (char *)malloc(strlen(str));
				if (data_arr[i] == NULL) {
					p_errors("memory allocation failed");
					return -1;
				}
					strcpy(data_arr[i], str); /*allocating memory for the new cell and copying the string into it */
					i++;
					data_arr = (char **)realloc(data_arr, (i + 1) * sizeof(char *));
					if (!data_arr) { /* Reallocating memory for a new space for the next cell */
						p_errors("memory allocation failed");
						return -1;
					}
			}
			/*skipping to the next argument after the end of the string */
			while (token != end_ptr && (&(token[strlen(token) - 1]) != end_ptr))
				token = strtok(NULL, " \t\n");
			token = strtok(NULL, " \t\n");
			continue;
		}
		/*if we found a comment or we're in a blank line we return the current size*/
		i = check_word(token, i);
		if (i <= 0)
			return -i;
		token = strtok(NULL, " \t\n"); /*gets the next argument */
	}
	return i;
}

 /**********************************************************************************************
 This function copies a string in case the string exist
 * @param line- the assembly line
 * @param str- the string will copy to this variable in if exists
 * @param ptr- a pointer to the begining of the string
 * @return 0 if the copy success or -1 if the is syntax error
  ********************************************************************************************/
int get_str(char *line, char *str, char *ptr) {
	int i;
	char *end_ptr;
	
	if (ptr==NULL)
		return 0;

	end_ptr = strrchr(line, '\"'); /*getting the ending quote of the string, if exists */
	if (end_ptr == ptr) {  /*if it doesn't exist, the result will be the beginning quote */
		p_errors("Missing \" before or after the string");
		return -1;
	}
	for (i = 0; i <= (end_ptr - ptr); i++) /*copying the string into str*/
		str[i] = ptr[i];
	str[i] = '\0';  /*adding end of string character*/
	
	return 0;
}

 /**********************************************************************************************
 This function checks an argument in the line
 * @param word- the argument that the function is going to check
 * @param idx- the current number of arguments
 * @return the number of argument after checking the word. In case there is a comment in the word,
	 the function return -1.
  ********************************************************************************************/
int check_word(char *word, int idx) {
	
	char args_arr[MAX_ARGS_LEN]; 
	int arg_idx = 0, comment=0;
	
	while (*word) { /*passing the word chars*/
		while (*word == ',') { /*commas are stored in a separate cell */
			data_arr[idx++] = ",";
			data_arr = (char **)realloc(data_arr, (idx + 1) * sizeof(char *));  /*reallocating memory for the next cell */
			if (!data_arr) {
				p_errors("memory allocation failed");
				return -1;
			}
			if (!(*(++word))) /* If the comma was at the end of the word, we finished check the word */
				return idx;
		}
		while ((*word) && (*word != ',') && (!comment)) {
	
			if (*word == ';') /*we reached a comment*/
				comment = -1;
			else {
				args_arr[arg_idx] = *(word++);
				arg_idx++;
			}
		}
		args_arr[arg_idx] = '\0'; /*adding end of string character */
		
		if (*args_arr) { /*if there's a new argument, we add it to the data array*/

			data_arr[idx] = (char*)malloc(strlen(args_arr));/*allocating memory for the new cell*/ 
			if (!data_arr[idx]) {
				p_errors("memory allocation failed");
				return -1;
			}
			
			strcpy(data_arr[idx++], args_arr); /*copying the argument into the new cell*/
		
			data_arr = (char **)realloc(data_arr, (idx + 1) * sizeof(char *)); /*reallocating new memory for the next cell*/
			if (!data_arr) { 
				p_errors("memory allocation failed");
				return -1;
			}
		}
		arg_idx = 0; /*init args_arr*/
		
	/* If we found a comment the actual line is over. In order to difference the value returned by the
	 function, we send the counter to the length until the comment */
		if (comment)
			return idx * comment;
	}
	return idx;
}
