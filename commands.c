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
    if(list == NULL)
    {
        printf("> List is corrupted, please reload it.\n");
        main_menu(list);
    }

    freeAddressBookList(list);
    printf("> The list is unloaded.\n");
}

void commandDisplay(AddressBookList * list)
{
    int phone_index;
    AddressBookNode * current_node;
    char * serialized_phones;

    if(list == NULL)
    {
        printf("> List is corrupted, please reload it.\n");
        main_menu(list);
    }

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
        serialized_phones = serialize_array(list, current_node, TRUE);

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
            "| Total phone book entries: %d \t\t\t|\n"
            "---------------------------------------------------------\n", (phone_index - 1)
    );

}

void commandForward(AddressBookList * list, int moves)
{
    int move_steps;

    if(list == NULL)
    {
        printf("> List is corrupted, please reload it.\n");
        main_menu(list);
    }

    if(moves < 1)
    {
        main_menu(list);
        printf("> Wrong input for moves, it should be greater than 1 anyway.\n");
    }
    else
    {
        printf("> Try moving forward for %d contacts...\n", moves);
    }

    /** Loop until it reaches the desired node, or if it can't, return an error message. */
    if(list->current->nextNode != NULL)
    {
        for(move_steps = 0; move_steps < moves; move_steps++)
        {
            if(list->current->nextNode == NULL)
            {
                printf("> No more entry exists!\n");
                main_menu(list);
            }

            list->current = list->current->nextNode;
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

    if(list == NULL)
    {
        printf("> List is corrupted, please reload it.\n");
        main_menu(list);
    }

    if(moves < 1)
    {
        main_menu(list);
        printf("> Wrong input for moves, it should be greater than 1 anyway.\n");
    }
    else
    {
        printf("> Try moving forward for %d contacts...\n", moves);
    }

    /** Loop until it reaches the desired node, or if it can't, return an error message. */
    if(list->current->previousNode != NULL)
    {
        for(move_steps = 0; move_steps < moves; move_steps++)
        {
            if(list->current->previousNode == NULL)
            {
                printf("> No more entry exists!\n");
                main_menu(list);
            }

            list->current = list->current->previousNode;
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
    if(list == NULL)
    {
        printf("> List is corrupted, please reload it.\n");
        main_menu(list);
    }

    if(id >= NODE_MINIMUM_ID && strlen(name) <= NAME_LENGTH)
    {
        /** Initialize a node and its array... */
        AddressBookNode * node = createAddressBookNode(id, name);
        node->array = createAddressBookArray();

        /** Add a phone number into it. */
        if(addTelephone(node->array, telephone) == TRUE && insertNode(list, node) == FALSE)
        {
            printf("> Insertion failed for ID %d and name %s!\n", id, name);
            main_menu(list);
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
    if(list == NULL)
    {
        printf("> List is corrupted, please reload it.\n");
        main_menu(list);
    }

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

    if(list == NULL)
    {
        printf("> List is corrupted, please reload it.\n");
        main_menu(list);
    }

    if(result_node != NULL)
    {
        printf("> Found %s in contact list. \n", name);
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
    if(list == NULL)
    {
        printf("> List is corrupted, please reload it.\n");
        main_menu(list);
    }

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
    if(list == NULL)
    {
        printf("> List is corrupted, please reload it.\n");
        main_menu(list);
    }

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

    /** Initialize a "node array" and something related... */
    AddressBookNode ** node_array;
    AddressBookNode * current_node;
    int array_index;
    array_index = 0;
    current_node = NULL;
    node_array = NULL;

    if(list == NULL)
    {
        printf("> List is corrupted, please reload it.\n");
        main_menu(list);
    }

    /** Detect the dependencies are valid or not */
    if((node_array = realloc(node_array, sizeof(*node_array) * list->size)) == NULL)
    {
        printf("> Reserves memory for node array failed.\n");
        main_menu(list);
    }
    if(list->head == NULL)
    {
        printf("> Contact list hasn't been loaded successfully. Please run \"load\" and try again.\n");
        main_menu(list);
    }
    else
    {
        current_node = list->head;
    }

    /** Put all nodes in to the array */
    while(current_node != NULL)
    {
        node_array[array_index] = current_node;
        current_node = current_node->nextNode;
        array_index++;
    }

    if(sort == &compareName)
    {
        printf("> Sorting by name...\n");
        sort_with_name(list, node_array, list->size, sort);
    }
    else
    {
        printf("> Sorting by ID...\n");
        sort_with_id(list, node_array, list->size, sort);
    }

    /** Now all things done, put back all result into the list */
    for(array_index = 0; array_index < list->size; array_index++)
    {
        /**
         * Set the new head and tail...
         * Head node does not have previous node, tail node does not have next node.
         * Since ANSI C / ISO C90 does not support dynamic switch-case (i.e. case must be a constant value),
         * so here I must use if-else. Looks more ugly but it's the only choice so far.
         * */
        if(array_index == 0)
        {
            list->head = node_array[array_index];
            node_array[array_index]->previousNode = NULL;
            node_array[array_index]->nextNode = node_array[array_index + 1];
        }
        else if(array_index == (list->size - 1))
        {
            list->tail = node_array[array_index];
            node_array[array_index]->nextNode = NULL;
            node_array[array_index]->previousNode = node_array[array_index - 1];
        }
        else
        {
            node_array[array_index]->nextNode = node_array[array_index + 1];
            node_array[array_index]->previousNode = node_array[array_index - 1];
        }
    }

    printf("> Done sorting process.\n");
}

int compareName(const void * node, const void * otherNode)
{
    /* Compare node name with otherNode name.
     * 
     * return < 0 when node name is smaller than otherNode name.
     * return 0 when the names are equal.
     * return > 0 when node name is bigger than otherNode name.
     */
    const char * str_a = *(const char**)node;
    const char * str_b = *(const char**)otherNode;

    return(strcmp(str_a, str_b));
}

int compareID(const void * node, const void * otherNode)
{
    /* Compare node id with otherNode id.
     * 
     * return < 0 when node id is smaller than otherNode id.
     * return 0 when the ids are equal.
     * return > 0 when node id is bigger than otherNode id.
     */

    int result;

    int id_a;
    int id_b;

    id_a = *(int *)node;
    id_b = *(int *)otherNode;

    result = id_a - id_b;

    if(result < 0)
    {
        return  -1;
    }
    else if(result > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void commandSave(AddressBookList * list, char * fileName)
{
    /** Initialize the file stuff */
    FILE * file;
    AddressBookNode * current_node;
    char * serialized_phones;
    int fclose_return_value;

    if(list == NULL)
    {
        printf("> List is corrupted, please reload it.\n");
        main_menu(list);
    }

    current_node = NULL;
    file = NULL;

    /** Detect if the list is valid or not... */
    if(list != NULL && list->head != NULL)
    {
        printf("> Opening file at %s...\n", fileName);
        current_node = list->head;
        file = fopen(fileName, "w");
    }
    else
    {
        printf("> Invalid list found. Try run \"load\" again before saving the list.\n");
        main_menu(list);
    }

    printf("> Writing content to file...\n");
    while(current_node != NULL)
    {
        /** Write to file buffer... */
        fprintf(file, "%d,%s", current_node->id, current_node->name);
        if(current_node->array->size > 0)
        {
            serialized_phones = serialize_array(list, current_node, FALSE);
            fprintf(file, ",%s\n", serialized_phones);
        }

        /** Process to the next one... */
        current_node = current_node->nextNode;
    }

    /** Finalize the process */
    printf("> Syncing cache and closing the file...\n");
    fclose_return_value = fclose(file);
    if(fclose_return_value != 0)
    {
        printf("> Save failed to path %s, fclose returns %d, not zero!\n", fileName, fclose_return_value);
        main_menu(list);
    }
    else
    {
        printf("> File %s has been created and saved successfully!\n", fileName);
    }
}



void parse_menu(char * user_input, AddressBookList * list)
{
    /** These are all some hard-coded if-elses, very long and dull lol */

    /** Prepare a string array to get the string token */
    char * split_token;

    /** Duplicate the input string to token first, or user_input itself will be polluted by "strtok" */
    split_token = malloc(sizeof(char) * strlen(user_input) + 1);
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
        printf("> Insertion complete.\n");
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

        if(list != NULL)
        {
            freeAddressBookList(list);
        }

        exit(0);
    }

    /** Parse quit, takes 1 argument */
    else if(strcmp(&split_token[0], COMMAND_SAVE) == 0 && count_space(user_input, 1, 0) == TRUE)
    {
        commandSave(list, parse_second_arg(list, split_token));
    }

    /** Parse sort, takes 1 argument */
    else if(strcmp(&split_token[0], COMMAND_SORT) == 0 && count_space(user_input, 1, 0) == TRUE)
    {
        parse_sort(list, parse_second_arg(list, split_token));
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

        if(split_token != NULL)
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

    char * line_to_parse;
    char * phone_newline_remove_token;

    /** Duplicate the input char to avoid pollutions and some other strange issues */
    if((line_to_parse = malloc(sizeof(char) * MAX_LINE_LENGTH)) == NULL)
    {
        printf("> Failed to parse insertion content, memory allocation failed.\n");
        main_menu(list);
    }
    else
    {
        strcpy(line_to_parse, second_arg);
    }

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
    parse_result = calloc((size_t)(comma_count + 1), sizeof(char*));
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
            }
        }
    }
}

char * serialize_array(AddressBookList * list, AddressBookNode * current_node, Boolean extra_space)
{
    /**
     * Nothing but just for serialize all the strings arrays from a node
     * and merge them into a single string with comma divided.
     * */
    int phone_index;
    char * serialized_phones;

    /** Serialize the phone number(s), plus 2 for null space char "\0". */
    if((serialized_phones =
                calloc(1,
                       (sizeof(current_node->array->telephones) * current_node->array->size + EXTRA_SPACES)
                )
       ) == NULL)
    {
        printf("> Memory allocation for phone text failed!\n");
        main_menu(list);
    }

    /**
     * Workaround: force clean up the string memory chunk before serialization
     * Sometimes the memory of this string may be duplicated with others, I have no idea about this.
     *    So just simply wipe it before using.
     * */
    memset(serialized_phones, 0, ((sizeof(current_node->array->telephones) * current_node->array->size) + 2));

    for(phone_index = 0; phone_index < current_node->array->size; phone_index++)
    {
        strcat(serialized_phones, current_node->array->telephones[phone_index]);

        /** The last string does not need to separate  */
        if(phone_index != (current_node->array->size - 1))
        {
            if(extra_space == TRUE)
            {
                strcat(serialized_phones, ", ");
            }
            else
            {
                strcat(serialized_phones, ",");
            }
        }
    }

    return serialized_phones;
}

void parse_sort(AddressBookList * list, char * second_arg)
{
    if(strcmp(second_arg, COMMAND_SORT_ID) == 0)
    {
        commandSort(list, compareID);
    }
    else if(strcmp(second_arg, COMMAND_SORT_NAME) == 0)
    {
        commandSort(list, compareName);
    }
    else
    {
        printf("> Invalid secondary sort argument \"%s\", it should be either \"name\" or \"id\".", second_arg);
        main_menu(list);
    }
}

AddressBookNode ** sort_with_id(AddressBookList * list, AddressBookNode ** node_array,
                                int array_length, int sort(const void * node, const void * otherNode))
{
    int * id_array;
    int array_index;

    id_array = malloc(sizeof(int) * array_length);

    /** Put node IDs into a integer array */
    for(array_index = 0; array_index < array_length; array_index++)
    {
        id_array[array_index] = node_array[array_index]->id;
    }

    /** Do sorting */
    qsort(id_array, (size_t)array_length, sizeof(int), sort);

    /** Put back the name into the nodes from name array */
    for(array_index = 0; array_index < array_length; array_index++)
    {
        node_array[array_index] = findByID(list, id_array[array_index]);
    }

    return node_array;

}

AddressBookNode ** sort_with_name(AddressBookList * list, AddressBookNode ** node_array,
                                  int array_length, int sort(const void * node, const void * otherNode))
{
    char ** name_array;
    int array_index;

    name_array = malloc(sizeof(*name_array) * array_length);

    /** Put node names into a name (string) array */
    for(array_index = 0; array_index < array_length; array_index++)
    {
        name_array[array_index] = node_array[array_index]->name;
    }

    /** Do sorting */
    qsort(name_array, (size_t)array_length, sizeof(*name_array), sort);

    /** Put back the name into the nodes from name array */
    for(array_index = 0; array_index < array_length; array_index++)
    {
        node_array[array_index] = findByName(list, name_array[array_index]);
    }

    return node_array;

}
