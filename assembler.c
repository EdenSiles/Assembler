#include "config.h"
#include "assembler.h"
#include "files.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    open_files(argc, argv); /*start the program by open the files*/
    frees_memory(); /*finish the program by frees all the memory allocation of global structures */
    return 0;
}
