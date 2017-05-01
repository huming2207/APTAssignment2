#include "helpers.h"

void readRestOfLine()
{
    int ch;
    while(ch = getc(stdin), ch != EOF && ch != '\n')
    { } /* Gobble each character. */

    /* Reset the error status of the stream. */
    clearerr(stdin);
}

void safe_free(void * ptr)
{
    /**
     * Some old compilers/standard libraries implementations
     * will crash the program if free() wipes a NULL pointer.
     *
     * In case this happens, I've implemented a safe-free here.
     * ONLY the non-null pointers will be executed.
     */
    if(ptr != NULL)
    {
        free(ptr);
    }
}

Boolean save_file(char * str_to_save, char * path, char * method)
{
    /** Initialize the file stuff */
    FILE * file;
    file = fopen(path, method);

    /** Check if it successfully set */
    if(file != NULL)
    {
        /** If fprintf return 0 then it is written successfully to file buffer */
        if(fprintf(file, "%s", str_to_save) == 0)
        {
            /** try saving the file */
            if(fclose(file) == 0)
            {
                fclose(file);
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
}

char * append_string(char * old_str, char * append_str)
{
    /** Initialize a new memory space for new string */
    char * new_str = malloc(strlen(old_str) + strlen(append_str) + NULL_SPACE);

    if(new_str == NULL)
    {
        return NULL;
    }

    /** Add a null char to it first (which will be appended to the last place later) */
    new_str[0] = '\0';

    /** Do concat now */
    strcat(new_str, old_str);
    strcat(new_str, append_str);

    return new_str;
}

char * get_user_input(int length)
{
    printf("Enter your command: ");

    /** Initialize something (of course), cast to size_t because CLion warning sucks. */
    char * user_input = malloc((size_t)(length + EXTRA_SPACES));

    if(user_input == NULL)
    {
        return NULL;
    }

    /** Do fgets to grab input */
    fgets(user_input, length, stdin);

    /** If it's not ended with new line character, then it must be overflowed */
    if(user_input[strlen(user_input) - 1] != '\n')
    {
        readRestOfLine();

        /* TODO: Should not just simply return NULL */
        return NULL;
    }

    /** Append a null character */
    strcat(user_input, "\0");

    return user_input;
}

int str_to_int(char * str)
{
    /** Get (only) the first int found from str */
    long num = strtol(str, NULL, 10);

    /** Return int, detect overflow before conversion */
    if(num > INT_MIN && num < INT_MAX)
    {
        return ((int)num);
    }
    else
    {
        return -32767;
    }
}
