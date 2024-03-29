#ifndef ADDRESSBOOK_LIST_H
#define ADDRESSBOOK_LIST_H

#include "addressbook_array.h"

#define NAME_LENGTH (20 + NULL_SPACE)

#define NODE_MINIMUM_ID 1

typedef struct addressBookNode
{
    int id;
    char name[NAME_LENGTH];
    AddressBookArray * array;
    struct addressBookNode * previousNode; /* The previous node in the linked-list */
    struct addressBookNode * nextNode; /* The next node in the linked-list */
} AddressBookNode;

typedef struct addressBookList
{
    int size;
    AddressBookNode * head;
    AddressBookNode * tail;
    AddressBookNode * current;
} AddressBookList;

AddressBookList * createAddressBookList();
void freeAddressBookList(AddressBookList * list);

AddressBookNode * createAddressBookNode(int id, char * name);
void freeAddressBookNode(AddressBookNode * node);

Boolean insertNode(AddressBookList * list, AddressBookNode * node);
Boolean deleteCurrentNode(AddressBookList * list);

Boolean forward(AddressBookList * list, int forward);
Boolean backward(AddressBookList * list, int backward);
AddressBookNode * findByID(AddressBookList * list, int id);
AddressBookNode * findByName(AddressBookList * list, char * name);

/** Some of my useful stuff begins below lol... */

/** Automatically get and set the head of list*/
Boolean set_list_head(AddressBookList * list);

/** Automatically get and set the tail of list*/
Boolean set_list_tail(AddressBookList * list);

/**
 * As a C# coder, I decide to make more syntactic sugars to make me feels comfortable lol...
 */
Boolean set_list_head_tail(AddressBookList * list);

#endif
