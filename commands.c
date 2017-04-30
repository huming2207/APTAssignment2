#include "commands.h"

/**
 * Note this file is to contain console output,
 * meaning printf's are expected to be seen in this file.
 */

AddressBookList * commandLoad(char * fileName)
{
    /**
     * Loads the given file and returns a new AddressBookList.
     * 
     * If the file doesn't exist or corruption is found in the file
     * then NULL is returned.
     */


    return NULL;
}

void commandUnload(AddressBookList * list)
{ }

void commandDisplay(AddressBookList * list)
{ }

void commandForward(AddressBookList * list, int moves)
{ }

void commandBackward(AddressBookList * list, int moves)
{ }

void commandInsert(AddressBookList * list, int id, char * name, char * telephone)
{ }

void commandAdd(AddressBookList * list, char * telephone)
{ }

void commandFind(AddressBookList * list, char * name)
{ }

void commandDelete(AddressBookList * list)
{ }

void commandRemove(AddressBookList * list, char * telephone)
{ }

void commandSort(
    AddressBookList * list,
    int sort(const void * node, const void * otherNode))
{
    /* Sort the nodes within list in ascending order using the
     * provided sort function to compare nodes.
     */
}

int compareName(const void * node, const void * otherNode)
{
    /* Compare node name with otherNode name.
     * 
     * return < 0 when node name is smaller than otherNode name.
     * return 0 when the names are equal.
     * return > 0 when node name is bigger than otherNode name.
     */
    return 0;
}

int compareID(const void * node, const void * otherNode)
{
    /* Compare node id with otherNode id.
     * 
     * return < 0 when node id is smaller than otherNode id.
     * return 0 when the ids are equal.
     * return > 0 when node id is bigger than otherNode id.
     */
    return 0;
}

void commandSave(AddressBookList * list, char * fileName)
{ }


void parse_menu(char * user_input, AddressBookList * list)
{
    /** These are all some hard-coded if-elses, very long and dull lol */

    /** Prepare a string array to get the string token */
    char * split_token = strtok(user_input, " ");

    /** Parse load, 1 argument is allowed */
    if(strcmp(&split_token[0], COMMAND_LOAD) == 0 && count_space(user_input, 1) == TRUE)
    {
        /** Parse the second token (file path) and initialize the list */
        list = commandLoad(parse_second_arg(split_token));
    }

    /** Parse unload, no argument is allowed */
    else if(strcmp(&split_token[0], COMMAND_UNLOAD) == 0 && count_space(user_input, 0) == TRUE)
    {
        commandUnload(list);
    }

    /** Parse display, no argument is allowed */
    else if(strcmp(&split_token[0], COMMAND_DISPLAY) == 0 && count_space(user_input, 0) == TRUE)
    {
        commandDisplay(list);
    }

    /** Parse forward, 1 argument is allowed */
    else if(strcmp(&split_token[0], COMMAND_FORWARD) == 0 && count_space(user_input, 1) == TRUE)
    {
        int step = str_to_int(parse_second_arg(split_token));
        commandForward(list, step);
    }

    /** Parse backward, 1 argument is allowed */
    else if(strcmp(&split_token[0], COMMAND_BACKWARD) == 0 && count_space(user_input, 1) == TRUE)
    {
        int step = str_to_int(parse_second_arg(split_token));
        commandBackward(list, step);
    }
}

Boolean count_space(char * user_input, int desired_spaces)
{
    /**
     * This function is used for some occasions where user mistakenly input
     * something else.
     *
     * For example, the unload command should only be "unload" itself.
     * If user input "unload test.txt" then it should print an invalid output.
     * */

    int user_input_length;
    int user_input_index;
    int space_count = 0;

    /** Set user input length to string's length (starts from 0) */
    user_input_length = (int)(strlen(user_input) - 1);

    for(user_input_index = 0; user_input_index < user_input_length; user_input_index++)
    {
        if(user_input[user_input_index] == ' ')
        {
            space_count++;
        }
    }

    if(space_count == desired_spaces)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

char * parse_second_arg(char * split_token)
{
    while(split_token != NULL)
    {
        split_token = strtok(NULL, " ");

        if(strlen(&split_token[0]) > 1)
        {
            return &split_token[0];
        }
        else
        {
            printf("\n> Invalid input.\n");

            /* TODO: Better not just return null, should call back the input function and get the input again. */
            return NULL;
        }
    }
}
