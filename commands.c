#include "commands.h"

/**
 * Note this file is to contain console output,
 * meaning printf's are expected to be seen in this file.
 */

void main_menu()
{
    /** Declare something useful... */
    AddressBookList * list;
    char * user_input;
    list = NULL;

    /**
     * 200 chars are quite enough for user input,
     * and don't worry, it will be cleaned later.
     *
     * If user input something even longer than this, it will be prevented in get_user_input().
     * */
    user_input = get_user_input(200);

    /** Parse all those commands */
    parse_menu(user_input, list);

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

    printf("> %d phone book entries have been loaded from the file.\n", line_count);
    printf("> Closing the file.\n");
    fclose(file);
    return list;
}

void commandUnload(AddressBookList * list)
{
    freeAddressBookList(list);
    printf("> The list is unloaded.\n");
}

void commandDisplay(AddressBookList * list)
{ }

void commandForward(AddressBookList * list, int moves)
{
    printf("Got %d moves\n", moves);
}

void commandBackward(AddressBookList * list, int moves)
{ }

void commandInsert(AddressBookList * list, int id, char * name, char * telephone)
{
    AddressBookNode * node = createAddressBookNode(id, name);

    insertNode(list, node);
}

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
    char * split_token;

    /** Duplicate the input string to token first, or user_input itself will be polluted by "strtok" */
    split_token = malloc(sizeof(user_input));
    strcpy(split_token, user_input);

    split_token = strtok(split_token, " ");

    /** Parse load, takes 1 argument */
    if(strcmp(&split_token[0], COMMAND_LOAD) == 0 && count_space(user_input, 1, 0) == TRUE)
    {
        /** Parse the second token (file path) and initialize the list */
        list = commandLoad(parse_second_arg(split_token));

        if(list == NULL)
        {
            /** No further error output is necessary, just reset it. */
            main_menu();
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
        int step = str_to_int(parse_second_arg(split_token));
        commandForward(list, step);
    }

    /** Parse backward, takes 1 argument */
    else if(strcmp(&split_token[0], COMMAND_BACKWARD) == 0 && count_space(user_input, 1, 0) == TRUE)
    {
        int step = str_to_int(parse_second_arg(split_token));
        commandBackward(list, step);
    }

    /** Parse insert, takes 3 arguments with comma separated */
    else if(strcmp(&split_token[0], COMMAND_INSERT) == 0 && count_space(user_input, 1, 2) == TRUE)
    {
        parse_insert(list, parse_second_arg(split_token));
    }

    /** Parse add, takes 1 argument  */
    else if(strcmp(&split_token[0], COMMAND_ADD) == 0 && count_space(user_input, 1, 0) == TRUE)
    {
        commandAdd(list, parse_second_arg(split_token));
    }

    /** Parse find, takes 1 argument */
    else if(strcmp(&split_token[0], COMMAND_FIND) == 0 && count_space(user_input, 1, 0) == TRUE)
    {
        commandFind(list, parse_second_arg(split_token));
    }

    /** Parse delete, no argument is allowed */
    else if(strcmp(&split_token[0], COMMAND_DELETE) == 0 && count_space(user_input, 0, 0) == TRUE)
    {
        commandDelete(list);
    }

    /** Parse remove, takes 1 argument */
    else if(strcmp(&split_token[0], COMMAND_REMOVE) == 0 && count_space(user_input, 1, 0) == TRUE)
    {
        commandRemove(list, parse_second_arg(split_token));
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
        main_menu();
    }
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

char * parse_second_arg(char * split_token)
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
            main_menu();

            /** For shutting up gcc only... */
            return NULL;
        }
    }

    /** Nothing but just shut up the compiler */
    return NULL;
}

void parse_insert(AddressBookList * list, char * second_arg)
{
    int split_index = 0;
    int comma_index = 0;
    int phone_append_index = 0;
    int comma_count = 0;
    int id = -1;
    char * contact_name;
    char * phone_array_str;
    char * split_token;
    char ** parse_result;
    char * line_to_parse;

    /** Duplicate the input char to avoid pollutions and some other strange issues */
    line_to_parse = malloc(strlen(second_arg) + EXTRA_SPACES);
    strcpy(line_to_parse, second_arg);

    /** Count how much comma exists */
    for(comma_index = 0; comma_index < strlen(line_to_parse); comma_index++)
    {
        if(line_to_parse[comma_index] == ',')
        {

            comma_count++;
        }
    }

    /** Should have move than 2 commas (3 arguments) anyway... */
    if(comma_count < 2)
    {
        printf("> Invalid input for address details! \n");
        main_menu();
    }

    /** Do memory (re)allocation for parse_result array itself */
    parse_result = malloc(sizeof(char*) * comma_count);

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
    if(parse_result[2] != NULL)
    {
        commandInsert(list, id, contact_name, parse_result[2]);
    }
    else
    {
        commandInsert(list, id, contact_name, EMPTY_STRING);
    }

    /** Append more telephones if exists and valid. */
    if(comma_count > 2)
    {
        for(phone_append_index = 0; phone_append_index <= comma_count; phone_append_index++)
        {
            if(parse_result[phone_append_index] != NULL)
            {
                commandAdd(list, parse_result[phone_append_index]);
            }
        }
    }
}
