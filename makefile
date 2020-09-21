# This is a Makefile for the mmn14 project
CC = gcc
CFLAGS = -g -Wall -ansi -pedantic
CCLINK = $(CC)
LIBS = -lm
OBJS = assembler.o check_line.o commands.o config.o def_op.o errors.o files.o first_pass.o labels.o second_pass.o word.o
RM = rm -f
# Creating the executable (mmn14)
mmn14: $(OBJS)
	$(CCLINK) -o mmn14 $(OBJS) $(LIBS)

# Creating object files using default rules

assembler.o: assembler.c config.h assembler.h files.h
check_line.o: check_line.c config.h assembler.h check_line.h
commands.o: commands.c config.h assembler.h commands.h word.h labels.h \
 def_op.h
config.o: config.c config.h assembler.h
def_op.o: def_op.c config.h assembler.h def_op.h commands.h labels.h
errors.o: errors.c config.h assembler.h labels.h
files.o: files.c assembler.h config.h files.h
first_pass.o: first_pass.c config.h assembler.h check_line.h labels.h \
 def_op.h
labels.o: labels.c config.h assembler.h labels.h word.h def_op.h \
 commands.h
second_pass.o: second_pass.c config.h assembler.h labels.h def_op.h \
 check_line.h
word.o: word.c config.h assembler.h word.h




# Cleaning old files before new make
clean:
	$(RM) mmn14 screen_test *.o *.bak *~ "#"* core
