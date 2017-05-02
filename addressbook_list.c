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

    AddressBookList * addressBookList = malloc(sizeof(*addressBookList));

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
    freeAddressBookNode(list->tail);
    freeAddressBookNode(list->head);
    freeAddressBookNode(list->current);

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
    AddressBookNode * addressBookNode = malloc(sizeof(*addressBookNode));

    /** Assign some values to it */
    addressBookNode->array = NULL;
    addressBookNode->id = id;
    addressBookNode->nextNode = NULL;
    addressBookNode->previousNode = NULL;

    return addressBookNode;
}

void freeAddressBookNode(AddressBookNode * node)
{
    /**
    * Free's the array within the node and the AddressBookNode itself.
    * 
    * Note the freeAddressBookArray(...) function is used to free the array.
    */

    /** Declare the nodes */
    AddressBookNode * next_node;
    AddressBookNode * previous_node;

    /** Free up the array */
    freeAddressBookArray(node->array);

    /** Clean up the address book node */
    next_node = node->nextNode;
    previous_node = node->previousNode;

    /** Wipe up all those nodes which in this one's FRONT position */
    while (next_node != NULL)
    {
        /** Selects current node and wait to clean */
        AddressBookNode * currentNode = next_node;

        /** Set the node position to the next one of itself ("next's next" lol) */
        next_node = next_node->nextNode;

        /** Then clean the current one */
        freeAddressBookArray(node->array);
        safe_free(currentNode);
    }

    /** Wipe up all those nodes which in this one's BACK position */
    while (previous_node != NULL)
    {
        /** Selects previous node and wait to clean */
        AddressBookNode * previousNode = previous_node;

        /** Set the node position to the previous one of itself ("previous's previous" lol) */
        previous_node = previous_node->previousNode;

        /** Then clean the current one */
        freeAddressBookArray(node->array);
        safe_free(previousNode);
    }

    /** Finally, clean up itself */
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

    /** As it said above, if a node with an existed ID, then return false */
    if (findByID(list, node->id) == NULL)
    {
        /** Get the "previous current" node */
        AddressBookNode * previous_current = list->current;

        if(previous_current != NULL)
        {
            /**
             * Set the "previous current" node's previous connection to the new node,
             *  Set the "previous current" node's previous node's next node, to the new node lol!
             **/
            AddressBookNode * previous_current_previous = previous_current->previousNode;
            previous_current->previousNode = node;

            if(previous_current_previous != NULL)
            {
                previous_current_previous->nextNode = node;
            }
        }

        /** Set the amount, plus one to append */
        list->size++;

        /** All done, set **real** current one to the new node */
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

    if (list->current != NULL || list->size != 0)
    {
        /** Find the current node's previous node */
        AddressBookNode * previous_current_node = list->current->previousNode;

        /** Find the current node's next node */
        AddressBookNode * next_current_node = list->current->nextNode;

        if (next_current_node != NULL)
        {
            /** Assign next node to previous node */
            next_current_node->previousNode = previous_current_node;
        }

        if (previous_current_node != NULL)
        {
            /** Set the current node to the originally previous one */
            list->current = previous_current_node;
        }
        else
        {
            /**
             * If originally previous one is null, set the current node to the originally next one
             * If originally next one is also null, then this list is empty
             * */
            if (next_current_node != NULL)
            {
                list->current = next_current_node;
            }
        }

        /** Wipe the current node */
        freeAddressBookNode(list->current);

        /** Set the amount */
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
