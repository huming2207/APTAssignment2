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

char * get_user_input(int length)
{
    /** Initialize something (of course), cast to size_t because CLion warning sucks. */
    char * user_input;
    user_input = malloc((size_t)(length + EXTRA_SPACES));

    printf("Enter your command: ");

    /** If user inputs nothing or some issues happens, return NULL to terminate the process */
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
        return NULL;
    }

    /** Append a null character */
    strcat(user_input, "\0");

    /** Do a trick to remove the new line character */
    user_input = strtok(user_input, "\n");

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

