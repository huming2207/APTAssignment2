#include "addressbook_list.h"

/**
* Note this file is *NOT* to contain console output,
* meaning printf's are *NOT* to be present in this file.
*/

AddressBookList * createAddressBookList()
{
    /**
    * Allocates and initialises a new AddressBookList.
    * malloc or calloc are expected to be used to create the
    * AddressBookList memory.
    * 
    * If memory cannot be allocated for the list NULL is returned.
    * 
    * Note head, tail and current should all be initialised to NULL.
    */
    AddressBookList * addressBookList;

    if((addressBookList = malloc(sizeof(AddressBookList))) == NULL)
    {
        printf("> Addressbook List memory allocation failed!\n");
        return NULL;
    }

    addressBookList->size = 0;
    addressBookList->current = NULL;
    addressBookList->head = NULL;
    addressBookList->tail = NULL;

    return addressBookList;
}

void freeAddressBookList(AddressBookList * list)
{
    /**
     * Free's all nodes within the list and the AddressBookList itself.
     * 
     * Note the freeAddressBookNode(...) function is used to free a node.
     */

    /** Free up each node */
    AddressBookNode * current_node;
    current_node = list->head;

    while(current_node != NULL)
    {
        freeAddressBookNode(current_node);
        current_node = current_node->nextNode;
    }

    list->size = 0;

    /** Wipe up itself */
    safe_free(list);

}

AddressBookNode * createAddressBookNode(int id, char * name)
{
    /**
    * Allocates and initialises a new AddressBookNode.
    * malloc or calloc are expected to be used to create the
    * AddressBookNode memory.
    * 
    * Note the array of the node should also be created and initialised,
    * which should be done with the createAddressBookArray(...) function.
    * 
    * If memory cannot be allocated for the node or array NULL is returned.
    * 
    * Note previousNode and nextNode should both be initialised to NULL.
    */

    /** Create an address book node */
    AddressBookNode * addressBookNode;
    if((addressBookNode = malloc(sizeof(*addressBookNode))) == NULL)
    {
        printf("Memory allocation for Addressbook node failed!\n");
        return NULL;
    }

    /** Assign some values to it */
    addressBookNode->array = NULL;
    addressBookNode->id = id;
    addressBookNode->nextNode = NULL;
    addressBookNode->previousNode = NULL;

    /** Initialize the name, duplicate it to prevent pollutions or other strange issues */
    memset(addressBookNode->name, 0, sizeof(addressBookNode->name));
    strcpy(addressBookNode->name, name);

    return addressBookNode;
}

void freeAddressBookNode(AddressBookNode * node)
{
    /**
    * Free's the array within the node and the AddressBookNode itself.
    * 
    * Note the freeAddressBookArray(...) function is used to free the array.
    */

    /** If this node's array is not null, wipe it up too. */
    if(node->array != NULL)
    {
        freeAddressBookArray(node->array);
    }

    /** Then wipe itself */
    safe_free(node);
}

Boolean insertNode(AddressBookList * list, AddressBookNode * node)
{
    /**
     * Inserts the node into the list and returns TRUE.
     * 
     * If the list already contains a node with the same id
     * then FALSE is returned and the node is not inserted.
     */
    AddressBookNode * previous_current_next;
    AddressBookNode * previous_current;

    /** As it said above, if a node with an existed ID, then return false */
    if (findByID(list, node->id) == NULL)
    {
        /** Get the "previous current" node */
        previous_current = list->current;

        /** Finalize previous assigns & merges */
        if(previous_current != NULL)
        {
            previous_current_next = previous_current->nextNode;

            previous_current->nextNode = node;

            if(previous_current_next != NULL)
            {
                previous_current_next->previousNode = node;
            }
        }

        /** Set the amount, plus one to append */
        list->size++;

        /** All done, set **real** current one to the new node */
        node->previousNode = previous_current;
        list->current = node;


        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

Boolean deleteCurrentNode(AddressBookList * list)
{
    /**
     * Delete's and free's the current node in the list and returns TRUE.
     * 
     * Note the freeAddressBookNode(...) function is used to free a node.
     * 
     * If the list has no nodes (i.e., there is no current node)
     * then FALSE is returned.
     */

    /**
     * Basically there are four situations:
     *
     * 1. Current node is between two non-null node
     * 2. Current node does not have its next node (tail node)
     * 3. Current node does not have its previous node (head node)
     * 4. Current node does not have both next node and previous node, i.e. it is alone, single, no friend, no family etc...
     *
     * By the way, is this one of the bonus mark lol?? Anyway I've done it and it is proved to work very smoothly. :)
     *
     * */
    AddressBookNode * current_node;

    if (list->current != NULL && list->size != 0)
    {
        if(list->current->nextNode != NULL && list->current->previousNode != NULL)
        {
            /** Situation #1 - Current node is between two non-null node */
            /** Break its connection, then connect the neighbour nodes together */
            current_node = list->current;
            current_node->previousNode->nextNode = current_node->nextNode;
            current_node->nextNode->previousNode = current_node->previousNode;
            list->current = current_node->nextNode;
            freeAddressBookNode(current_node);
        }
        else if(list->current->nextNode == NULL && list->current->previousNode != NULL)
        {
            /** Situation #2 - Current node does not have its next node (tail node) */
            /** Break its connection, then connect the previous node's next node to NULL */
            current_node = list->current;
            current_node->previousNode->nextNode = NULL;
            list->current = current_node->previousNode;
            freeAddressBookNode(current_node);
        }
        else if(list->current->nextNode != NULL && list->current->previousNode == NULL)
        {
            /** Situation #3 - Current node does not have its previous node (head node) */
            /** Break its connection, then connect the next node's previous node to NULL */
            current_node = list->current;
            current_node->nextNode->previousNode = NULL;
            list->current = current_node->nextNode;
            freeAddressBookNode(current_node);
        }
        else
        {
            /** Situation #4 - Current node does not have both next node and previous node */
            freeAddressBookNode(list->current);
        }

        /** Shrink the size of the list */
        list->size--;
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

Boolean forward(AddressBookList * list, int forward)
{
    /**
     * Moves the current node forward in the list by the number provided
     * and returns TRUE.
     * 
     * If the current node cannot be moved forward by that many positions
     * then FALSE is returned and current remains unchanged.
     */

    /** Assign the current node to default current node */
    AddressBookNode * current_node = list->current;

    /** Assign a step index */
    int step_index = 0;

    /** Loop the index to the desired value */
    while (step_index < forward)
    {
        /** Judge if it can be forward */
        if (current_node->nextNode != NULL)
        {
            /** Forward it and return true */
            current_node = current_node->nextNode;
            step_index++;
        }
        else
        {
            return FALSE;
        }
    }

    return TRUE;

}

Boolean backward(AddressBookList * list, int backward)
{
    /**
    * Moves the current node backward in the list by the number provided
    * and returns TRUE.
    * 
    * If the current node cannot be moved backward by that many positions
    * then FALSE is returned and current remains unchanged.
    */

    /** Assign the current node to default current node */
    AddressBookNode * current_node = list->current;

    /** Assign a step index */
    int step_index = 0;

    /** Loop the index to the desired value */
    while (step_index < backward)
    {
        /** Judge if it can be forward */
        if (current_node->previousNode != NULL)
        {
            /** Forward it and return true */
            current_node = current_node->previousNode;
            step_index++;
        }
        else
        {
            return FALSE;
        }
    }

    return TRUE;
}

AddressBookNode * findByID(AddressBookList * list, int id)
{
    /**
     * Returns the node that matches the id provided.
     * 
     * If no node with a matching id exists then NULL is returned.
     */

    /** Declare a node pointer and set to head position */
    AddressBookNode * node_query = list->head;

    /** If the node is null (even not initialized), return NULL result and stop the process */
    if(node_query == NULL)
    {
        return NULL;
    }

    /** If it's lucky enough and the this list's head node is what it wants, simply return it. */
    if (node_query->id == id)
    {
        return node_query;
    }

    /** If it's not lucky, do some dirty works instead! */
    while (node_query->nextNode != NULL)
    {
        /** If it's lucky enough and the this next node is what it wants, simply return it. */
        if (node_query->nextNode->id == id)
        {
            /** Set the next node to the node it needs and return */
            node_query = node_query->nextNode;
            return node_query;
        }
        else
        {
            /** If it's STILL NOT lucky, continue to search,
             * ...just like a male software engineering student keeps seeking for a girlfriend! */
            node_query = node_query->nextNode;
        }
    }

    /** If there is actually NOTHING, then return a NULL to finish the process. */
    return NULL;
}

AddressBookNode * findByName(AddressBookList * list, char * name)
{
    /**
    * Sets current to the first node that matches the name provided
    * and returns this node.
    * 
    * If no node with a matching name exists then NULL is returned
    * and current remains unchanged.
    */

    /** Declare a node pointer and set to head position */
    AddressBookNode * node_query = list->head;

    /** If the node is null (even not initialized), return NULL result and stop the process */
    if(node_query == NULL)
    {
        return NULL;
    }

    /** If it's lucky enough and the this list's head node is what it wants, simply return it. */
    if (strcmp(node_query->name, name) == 0)
    {
        return node_query;
    }

    /** If it's not lucky, do some dirty works instead! */
    while (node_query->nextNode != NULL)
    {
        /** If it's lucky enough and the this next node is what it wants, simply return it. */
        if (strcmp(node_query->nextNode->name, name) == 0)
        {
            /** Set the next node to the node it needs and return */
            node_query = node_query->nextNode;
            return node_query;
        }
        else
        {
            /** If it's STILL NOT lucky, continue to search,
             * ...just like a male software engineering student keeps seeking for a girlfriend! */
            node_query = node_query->nextNode;
        }
    }

    /** When there IS NOTHING, return NULL */
    return NULL;
}

Boolean set_list_head(AddressBookList * list)
{
    AddressBookNode * current_node;
    current_node = list->current;

    /** Return true only if the list's current node is set */
    if(current_node != NULL)
    {
        /** Loop until finds the "last next" node */
        while(current_node->previousNode != NULL)
        {
            current_node = current_node->previousNode;
        }

        list->head = current_node;

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

Boolean set_list_tail(AddressBookList * list)
{
    AddressBookNode * current_node;
    current_node = list->current;

    /** Return true only if the list's current node is set */
    if(current_node != NULL)
    {
        /** Loop until finds the "last previous" node */
        while(current_node->nextNode != NULL)
        {
            current_node = current_node->nextNode;
        }

        list->tail = current_node;

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

Boolean set_list_head_tail(AddressBookList * list)
{
    /**
     * Set both head and tail of the list.
     *
     * As a C# coder, I decide to make more syntactic sugars to make me feels comfortable lol...
     */

    if(set_list_head(list) == TRUE && set_list_tail(list) == TRUE)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
