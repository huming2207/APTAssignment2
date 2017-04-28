#include "addressbook_array.h"

/**
* Note this file is NOT to contain console output,
* meaning printf's are NOT to be present in this file.
*/

AddressBookArray * createAddressBookArray()
{
    /**
     * Allocates and initialises a new AddressBookArray.
     * malloc or calloc are expected to be used to create the
     * AddressBookArray memory.
     * 
     * If memory cannot be allocated for the array NULL is returned.
     * 
     * Note telephones should be initialised to NULL.
     */

    /** Allocate a memory area */
    AddressBookArray * addressBookArray = malloc(sizeof(*addressBookArray));

    /** Assign some empty stuff */
    addressBookArray->telephones = NULL;
    addressBookArray->size = 0;

    return addressBookArray;
}

void freeAddressBookArray(AddressBookArray * array)
{
    /**
     * Free's all telephones within the array and the AddressBookArray itself.
     */

    /** Declare the telephone detail nodes */
    char ** telephoneNodes = array->telephones;
    int nodeIndex;

    /** Do a for-loop to wipe the memory for the inner contents of node array ("char*" strings); */
    for(nodeIndex = 0; nodeIndex < array->size; nodeIndex++)
    {
        safe_free(telephoneNodes[nodeIndex]);
    }

    /** ...then wipe the telephone array; */
    safe_free(telephoneNodes);

    /** ...finally, wipe the array itself. */
    safe_free(array);

}

Boolean addTelephone(AddressBookArray * array, char * telephone)
{
    /**
     * Adds the provided telephone to the telephones array and returns TRUE.
     * 
     * If the telephone already exists or memory cannot be allocated
     * then FALSE is returned.
     * 
     * malloc or calloc are expected to be used to create memory for
     * the new telephone - do not forget to copy the provided telephone over.
     * Here is some sample code:
     * 
     * char * newTelephone = malloc(TELEPHONE_LENGTH);
     * strcpy(newTelephone, telephone);
     * 
     * After this it is expected you use *realloc* to make the telephones
     * array grow by 1. Here is some sample code:
     * 
     * array->telephones = realloc(array->telephones,
     *                     sizeof(*array->telephones) * (array->size + 1));
     * 
     * Once the array has grown by 1 you can store the new telephone in it
     * at the end. Here is some sample code:
     * 
     * array->telephones[array->size] = newTelephone;
     * array->size++;
     */

    /** Allocate memory for new telephone chars */
    char * new_phone = malloc(TELEPHONE_LENGTH);

    /** Detect if memory allocation works or not, if not, return FALSE */
    if(new_phone != NULL)
    {
        strcpy(new_phone, telephone);
    }
    else
    {
        return FALSE;
    }

    /** Expand the phone array index */
    array->size++;

    /** Expand the list itself */
    array->telephones = realloc(array->telephones, sizeof(*array->telephones) * (array->size));

    /** Detect if memory reallocation works or not, if not, return FALSE */
    if(array->telephones != NULL)
    {
        array->telephones[array->size - 1] = new_phone;
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

Boolean removeTelephone(AddressBookArray * array, char * telephone)
{
   /**
    * Removes the provided telephone from the telephones array and
    * returns TRUE.
    * 
    * If the telephone does not exist or memory cannot be reallocated
    * then FALSE is returned.
    * 
    * It is expected you use *realloc* to shrink the telephones
    * array by 1. Here is some sample code:
    * 
    * array->telephones = realloc(array->telephones,
    *                     sizeof(*array->telephones) * (array->size - 1));
    * array->size--;
    * 
    * Note for this to work you will first have to ensure the telephone
    * to remove is at the end of the array.
    * 
    * Also note you will need to safe_free(...) the removed telephone as well.
    * 
    * If you are removing the LAST telephone in the array then you will need to
    * free telephones itself and set it to NULL. Here is some sample code:
    * 
    * safe_free(array->telephones);
    * array->telephones = NULL;
    */

    int search_index;

    for(search_index = 0; search_index < array->size; search_index++)
    {
        /** If the string matches, do the removal */
        if(strcmp(array->telephones[search_index], telephone) == 0)
        {
            /** Remove the telephone string */
            safe_free(array->telephones[search_index]);

            /** Shrink the whole telephone list index */
            array->size--;

            /** Shrink the whole telephone list */
            array->telephones = realloc(array->telephones, sizeof(*array->telephones) * (array->size));
        }
    }

    /** If the array size is 0 (empty array), do the whole removal of the array, dispose it. */
    if(array->size == 0)
    {

    }

    return FALSE;
}

char * findTelephone(AddressBookArray * array, char * telephone)
{
    /**
     * Returns the telephone that matches the telephone provided.
     * 
     * If no telephone exists then NULL is returned.
     */

    return NULL;
}
