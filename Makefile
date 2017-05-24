SOURCES=addressbook.c addressbook_list.c addressbook_array.c commands.c helpers.c
HEADERS=addressbook.h addressbook_list.h addressbook_array.h commands.h helpers.h
PROGRAM=addressbook
FLAGS=-ansi -pedantic -Wall

all:
	gcc $(FLAGS) -o $(PROGRAM) $(SOURCES)

clean:
	rm $(PROGRAM)

debug:
	gcc $(FLAGS) -g -o $(PROGRAM) $(SOURCES)

mac-gcc:
	gcc-7 $(FLAGS) -o  $(PROGRAM) $(SOURCES)

mac-gcc-debug:
	gcc-7 $(FLAGS) -g -o  $(PROGRAM) $(SOURCES)

archive:
	zip $(USER)-a2 $(SOURCES) $(HEADERS) Makefile
