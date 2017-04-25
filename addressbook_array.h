#ifndef ADDRESSBOOK_ARRAY_H
#define ADDRESSBOOK_ARRAY_H

#include "helpers.h"

#define TELEPHONE_LENGTH (10 + NULL_SPACE)

typedef struct addressBookArray
{
    int size;
    char ** telephones;
} AddressBookArray;

AddressBookArray * createAddressBookArray();
void freeAddressBookArray(AddressBookArray * array);

Boolean addTelephone(AddressBookArray * array, char * telephone);
Boolean removeTelephone(AddressBookArray * array, char * telephone);

char * findTelephone(AddressBookArray * array, char * telephone);

#endif
