#include "commands.h"

/**
 * Note this file is to contain console output,
 * meaning printf's are expected to be seen in this file.
 */

void main_menu(AddressBookList * list)
{
    /** Declare something useful... */
    char * user_input;

    /**
     * 200 chars are quite enough for user input,
     * and don't worry, it will be cleaned later.
     *
     * If user input something even longer than this, it will be prevented in get_user_input().
     * */
    user_input = get_user_input(200);

    if(user_input == NULL)
    {
        printf("> Parsing user input failed, please try again!\n");
        main_menu(list);
    }
    else
    {
        /** Parse all those commands */
        parse_menu(user_input, list);
    }



    clean_user_input_buffer(user_input);

}

AddressBookList * commandLoad(char * fileName)
{
    /**
     * Loads the given file and returns a new AddressBookList.
     * 
     * If the file doesn't exist or corruption is found in the file
     * then NULL is returned.
     */

    AddressBookList * list;
    FILE * file;
    char line_buffer[MAX_LINE_LENGTH];
    int line_count = 0;

    /** Try load the file */
    file = fopen(fileName, "r");
    printf("> Opening the file at %s \n", fileName);

    if(file == NULL)
    {
        printf("> Error: Unable to find the specified file.\n");
        return NULL;
    }
    else
    {
        printf("> Loading the file ...\n");
    }

    /** Initially create the book list */
    list = createAddressBookList();

    /** Read line by line until the end */
    while(fgets(line_buffer, MAX_LINE_LENGTH, file) != NULL)
    {
        if(line_buffer[0] != '#')
        {
            line_count++;
            parse_insert(list, line_buffer);
        }
    }

    /** Finalize the file loading process */
    printf("> %d phone book entries have been loaded from the file.\n", line_count);
    printf("> Closing the file.\n");
    fclose(file);

    /** Finally, initialize the head and tail of the list */
    set_list_head_tail(list);

    return list;
}

void commandUnload(AddressBookList * list)
{
    freeAddressBookList(list);

    printf("> The list is unloaded.\n");
}

void commandDisplay(AddressBookList * list)
{
    int phone_index;
    AddressBookNode * current_node;
    char * serialized_phones;

    serialized_phones = NULL;
    current_node = list->head;
    phone_index = 1;

    if(current_node == NULL)
    {
        printf("> Address book is empty!\n");
        main_menu(list);
    }

    /** Print header of the list */
    printf(
            "\n\n---------------------------------------------------------\n"
            "|\tPos\t|\tSerial\t|\tID\t|\tName\t|\tTelephone\t|\n"
            "---------------------------------------------------------\n"
    );

    /** Print the main content of the list */

    while(current_node != NULL)
    {
        serialized_phones = serialize_array(list, current_node);

        if(current_node == list->current)
        {
            printf("|\t%s\t|\t%d\t|\t%d\t|\t%s\t|\t%s\t|\n",
                   "CR", phone_index, current_node->id, current_node->name, serialized_phones);
        }
        else
        {
            printf("|\t%s\t|\t%d\t|\t%d\t|\t%s\t|\t%s\t|\n",
                   "  ", phone_index, current_node->id, current_node->name, serialized_phones);
        }

        /** Loop to next node */
        phone_index++;
        current_node = current_node->nextNode;
    }

    /** Print footer of the list */
    printf(
            "---------------------------------------------------------\n"
            "| Total phone book entries: %d \t\t|\n"
            "---------------------------------------------------------\n", (phone_index - 1)
    );

}

void commandForward(AddressBookList * list, int moves)
{
    int move_steps;
    AddressBookNode * current_node;
    current_node = list->current;

    /** Loop until it reaches the desired node, or if it can't, return an error message. */
    if(current_node != NULL)
    {
        for(move_steps = 0; move_steps < moves; move_steps++)
        {
            if(current_node == NULL)
            {
                printf("> No more entry exists!\n");
                main_menu(list);
            }

            current_node = current_node->nextNode;
        }
    }
    else
    {
        printf("> No more entry exists!\n");
        main_menu(list);
    }
}

void commandBackward(AddressBookList * list, int moves)
{
    int move_steps;
    AddressBookNode * current_node;
    current_node = list->current;

    /** Loop until it reaches the desired node, or if it can't, return an error message. */
    if(current_node != NULL)
    {
        for(move_steps = 0; move_steps < moves; move_steps++)
        {
            if(current_node == NULL)
            {
                printf("> No more entry exists!\n");
                main_menu(list);
            }

            current_node = current_node->previousNode;
        }
    }
    else
    {
        printf("> No more entry exists!\n");
        main_menu(list);
    }
}

void commandInsert(AddressBookList * list, int id, char * name, char * telephone)
{
    if(id >= NODE_MINIMUM_ID && strlen(name) <= NAME_LENGTH)
    {
        /** Initialize a node and its array... */
        AddressBookNode * node = createAddressBookNode(id, name);
        node->array = createAddressBookArray();

        /** Add a phone number into it. */
        if(addTelephone(node->array, telephone) == TRUE && insertNode(list, node) == TRUE)
        {
            printf("> Insertion complete.\n");
        }
        else
        {
            printf("> Insertion failed for ID %d and name %s!\n", id, name);
        }


    }
    else
    {
        printf("> Invalid length range for name %s or ID %d!\n", name, id);
        main_menu(list);
    }
}

void commandAdd(AddressBookList * list, char * telephone)
{
    if(addTelephone(list->current->array, telephone) == TRUE)
    {
        printf("> Phone number %s has been added.\n", telephone);
    }
    else
    {
        printf("> Cannot add phone number %s !\n", telephone);
    }
}

void commandFind(AddressBookList * list, char * name)
{
    AddressBookNode * result_node;
    result_node = findByName(list, name);

    if(result_node != NULL)
    {
        list->current = result_node;
    }
    else
    {
        printf("> Cannot find any entry with name %s !\n", name);
        main_menu(list);
    }
}

void commandDelete(AddressBookList * list)
{
    if(deleteCurrentNode(list) == TRUE)
    {
        printf("> Current node has been deleted.\n");
    }
    else
    {
        printf("> Cannot delete current node!\n");
    }
}

void commandRemove(AddressBookList * list, char * telephone)
{
    if(removeTelephone(list->current->array, telephone) == TRUE)
    {
        printf("> Phone number %s has been removed.\n", telephone);
    }
    else
    {
        printf("> Cannot remove phone number %s !\n", telephone);
    }
}

void commandSort(AddressBookList * list, int sort(const void * node, const void * otherNode))
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
    char * split_token;

    /** Duplicate the input string to token first, or user_input itself will be polluted by "strtok" */
    split_token = malloc(sizeof(user_input));
    strcpy(split_token, user_input);

    split_token = strtok(split_token, " ");

    /** Parse load, takes 1 argument */
    if(strcmp(&split_token[0], COMMAND_LOAD) == 0 && count_space(user_input, 1, 0) == TRUE)
    {
        /** Parse the second token (file path) and initialize the list */
        list = commandLoad(parse_second_arg(list, split_token));

        if(list == NULL)
        {
            /** No further error output is necessary, just reset it. */
            main_menu(list);
        }
    }

    /** Parse unload, no argument is allowed */
    else if(strcmp(&split_token[0], COMMAND_UNLOAD) == 0 && count_space(user_input, 0, 0) == TRUE)
    {
        commandUnload(list);
    }

    /** Parse display, no argument is allowed */
    else if(strcmp(&split_token[0], COMMAND_DISPLAY) == 0 && count_space(user_input, 0, 0) == TRUE)
    {
        commandDisplay(list);
    }

    /** Parse forward, takes 1 argument */
    else if(strcmp(split_token, COMMAND_FORWARD) == 0 && count_space(user_input, 1, 0) == TRUE)
    {
        int step = str_to_int(parse_second_arg(list, split_token));
        commandForward(list, step);
    }

    /** Parse backward, takes 1 argument */
    else if(strcmp(&split_token[0], COMMAND_BACKWARD) == 0 && count_space(user_input, 1, 0) == TRUE)
    {
        int step = str_to_int(parse_second_arg(list, split_token));
        commandBackward(list, step);
    }

    /** Parse insert, takes 3 arguments with comma separated */
    else if(strcmp(&split_token[0], COMMAND_INSERT) == 0 && count_space(user_input, 1, 2) == TRUE)
    {
        parse_insert(list, parse_second_arg(list, split_token));
    }

    /** Parse add, takes 1 argument  */
    else if(strcmp(&split_token[0], COMMAND_ADD) == 0 && count_space(user_input, 1, 0) == TRUE)
    {
        commandAdd(list, parse_second_arg(list, split_token));
    }

    /** Parse find, takes 1 argument */
    else if(strcmp(&split_token[0], COMMAND_FIND) == 0 && count_space(user_input, 1, 0) == TRUE)
    {
        commandFind(list, parse_second_arg(list, split_token));
    }

    /** Parse delete, no argument is allowed */
    else if(strcmp(&split_token[0], COMMAND_DELETE) == 0 && count_space(user_input, 0, 0) == TRUE)
    {
        commandDelete(list);
    }

    /** Parse remove, takes 1 argument */
    else if(strcmp(&split_token[0], COMMAND_REMOVE) == 0 && count_space(user_input, 1, 0) == TRUE)
    {
        commandRemove(list, parse_second_arg(list, split_token));
    }

    /** Parse quit, no argument is allowed  */
    else if(strcmp(&split_token[0], COMMAND_QUIT) == 0 && count_space(user_input, 0, 0) == TRUE)
    {
        printf("> Goodbye.\n");
        exit(0);
    }

    /** For the else things, it must be WRONG, return error message then... */
    else
    {
        printf("> Invalid input! \n");
        main_menu(list);
    }

    main_menu(list);
}

Boolean count_space(char * user_input, int desired_spaces, int desired_commas)
{
    /**
     * This function is used for some occasions where user mistakenly input
     * something else.
     *
     * For example, the unload command should only be "unload" itself.
     * If user input "unload test.txt" then it should print an invalid output.
     *
     * Also for example, if the user type in "insert 123,foobar",
     * it is also wrong because there is no phone number available.
     * */

    int user_input_length;
    int user_input_index;
    int space_count = 0;
    int comma_count = 0;

    /** Set user input length to string's length (starts from 0) */
    user_input_length = (int)(strlen(user_input) - 1);

    for(user_input_index = 0; user_input_index < user_input_length; user_input_index++)
    {
        if(user_input[user_input_index] == ' ')
        {
            space_count++;
        }
        else if(user_input[user_input_index] == ',')
        {
            comma_count++;
        }
    }

    if(space_count == desired_spaces && comma_count == desired_commas)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

char * parse_second_arg(AddressBookList * list, char * split_token)
{
    while(split_token != NULL)
    {
        split_token = strtok(NULL, " ");

        if(strlen(split_token) > 1)
        {
            return split_token;
        }
        else
        {
            printf("> Invalid input.\n");
            main_menu(list);

            /** For shutting up gcc only... */
            return NULL;
        }
    }

    /** Nothing but just shut up the compiler */
    return NULL;
}

void parse_insert(AddressBookList * list, char * second_arg)
{
    int comma_count = 0;
    int comma_index = 0;
    int phone_append_index = 0;
    int split_index = 0;
    int id;
    char * contact_name;
    char * split_token;
    char ** parse_result; /** Initialize a "string array" to deal with the parsing separation */

    char line_to_parse[MAX_LINE_LENGTH];
    char * phone_newline_remove_token;

    /** Duplicate the input char to avoid pollutions and some other strange issues */
    strcpy(line_to_parse, second_arg);


    /** Count how much comma exists */
    for(comma_index = 0; comma_index < strlen(line_to_parse); comma_index++)
    {
        if(line_to_parse[comma_index] == ',')
        {

            comma_count++;
        }
    }

    /** Should have move than 1 commas (2 arguments) anyway... */
    if(comma_count < 1)
    {
        printf("> Invalid input for address details! \n");
        main_menu(list);
    }

    /** Do memory (re)allocation for parse_result array itself */
    parse_result = malloc(sizeof(char*) * (comma_count + 1));

    split_token = strtok(line_to_parse, ",");

    while(split_token != NULL)
    {
        if(split_index <= comma_count)
        {
            parse_result[split_index] = split_token;
            split_index++;
        }

        split_token = strtok(NULL, ",");
    }

    /** Parse ID and contact name */
    id = str_to_int(parse_result[0]);
    contact_name = parse_result[1];

    /** Run insertion */
    if(comma_count >= 2)
    {
        phone_newline_remove_token = strtok(parse_result[2], "\n");
        commandInsert(list, id, contact_name, phone_newline_remove_token);
        phone_newline_remove_token = NULL;
    }
    else
    {
        contact_name = strtok(contact_name, "\n");
        commandInsert(list, id, contact_name, EMPTY_STRING);
    }

    /** Append more telephones if exists and valid. */
    if(comma_count > 2)
    {
        for(phone_append_index = 3; phone_append_index <= comma_count; phone_append_index++)
        {
            if(parse_result[phone_append_index] != NULL)
            {
                phone_newline_remove_token = strtok(parse_result[phone_append_index], "\n");
                addTelephone(list->current->array, phone_newline_remove_token);
                phone_newline_remove_token = NULL;
            }
        }
    }
}

char * serialize_array(AddressBookList * list, AddressBookNode * current_node)
{
    /**
     * Nothing but just for serialize all the strings arrays from a node
     * and merge them into a single string with comma divided.
     * */
    int phone_index;
    char * serialized_phones;
    serialized_phones = NULL;

    for(phone_index = 0; phone_index < current_node->array->size; phone_index++)
    {
        /** Serialize the phone number(s) */
        if((serialized_phones = malloc(sizeof(current_node->array->telephones) * current_node->array->size)) == NULL)
        {
            printf("> Memory allocation for phone text failed!\n");
            main_menu(list);
        }

        strcat(serialized_phones, current_node->array->telephones[phone_index]);

        /** The last string does not need to separate  */
        if(phone_index != (current_node->array->size - 1))
        {
            strcat(serialized_phones, ", ");
        }
    }

    return serialized_phones;
}
