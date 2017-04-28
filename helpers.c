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