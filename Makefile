SOURCES=addressbook.c addressbook_list.c addressbook_array.c commands.c helpers.c
HEADERS=addressbook.h addressbook_list.h addressbook_array.h commands.h helpers.h
PROGRAM=addressbook
FLAGS=-ansi -pedantic -Wall

all:
	gcc $(FLAGS) -o $(PROGRAM) $(SOURCES)

clean:
	rm $(PROGRAM)

archive:
	zip $(USER)-a2 $(SOURCES) $(HEADERS) Makefile
