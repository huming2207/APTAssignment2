#ifndef COMMANDS_H
#define COMMANDS_H

#include "addressbook_list.h"

#define MAX_LINE_LENGTH 512

#define COMMAND_LOAD "load"
#define COMMAND_UNLOAD "unload"
#define COMMAND_DISPLAY "display"
#define COMMAND_FORWARD "forward"
#define COMMAND_BACKWARD "backward"
#define COMMAND_INSERT "insert"
#define COMMAND_ADD "add"
#define COMMAND_FIND "find"
#define COMMAND_DELETE "delete"
#define COMMAND_REMOVE "remove"
#define COMMAND_SORT "sort"
#define COMMAND_SORT_NAME "name"
#define COMMAND_SORT_ID "id"
#define COMMAND_SAVE "save"
#define COMMAND_QUIT "quit"

AddressBookList * commandLoad(char * fileName);
void commandUnload(AddressBookList * list);
void commandDisplay(AddressBookList * list);
void commandForward(AddressBookList * list, int moves);
void commandBackward(AddressBookList * list, int moves);
void commandInsert(AddressBookList * list, int id, char * name, char * telephone);
void commandAdd(AddressBookList * list, char * telephone);
void commandFind(AddressBookList * list, char * name);
void commandDelete(AddressBookList * list);
void commandRemove(AddressBookList * list, char * telephone);
void commandSort(
    AddressBookList * list,
    int sort(const void * node, const void * otherNode));
int compareName(const void * node, const void * otherNode);
int compareID(const void * node, const void * otherNode);
void commandSave(AddressBookList * list, char * fileName);

#endif
