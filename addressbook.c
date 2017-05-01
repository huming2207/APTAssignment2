#include "addressbook.h"

/**
* This file is to contain the main menu loop and thus it will have console
* output, meaning printf's are expected to be seen in this file.
* 
* After the user enters a command, then the corresponding function is to be
* called from commands.c, e.g., "forward" corresponds to "commandForward(...)".
*/

int main(int argc, char ** argv)
{
    /** Yea actually... I did some printf as you required above, didn't I lol?! */
    const char * studentInfo =
            "\n\n-----------------------------------------------------------------\n"
                    "Student name: Ming Hu\n"
                    "Student number: 3554025\n"
                    "Advanced Programming Techniques, Assignment Two, Semester 1, 2017\n"
                    "-----------------------------------------------------------------\n\n";

    printf("%s", studentInfo);

    main_menu();
    return EXIT_SUCCESS;
}
