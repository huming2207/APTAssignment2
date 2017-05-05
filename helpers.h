#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <assert.h>
#include <limits.h>

typedef enum boolean
{
    FALSE = 0,
    TRUE
} Boolean;



#define NEW_LINE_SPACE 1
#define NULL_SPACE 1

/**
 * This is used to compensate for the extra character spaces taken up by
 * the '\n' and '\0' when input is read through fgets().
 **/
#define EXTRA_SPACES (NEW_LINE_SPACE + NULL_SPACE)

#define EMPTY_STRING ""

/**
 * Call this function whenever you detect buffer overflow.
 **/
void readRestOfLine();

/**
 * Do a safe delete
 * */
void safe_free(void * ptr);


/**
 * Get user's input string
 */
char * get_user_input(int length);

/**
 * Converting string to int
 */
int str_to_int(char * str);

#endif
