#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SH_NAME toyshell

int main (int argc, char *argv []){
    
    
    /* get shell name */

    /* get the terminator */

    /* initialize the counter */
    int counter = 1;
    

    return 0;

}


#define MAX_TOKENS 1024

void TokenizeCommandLine (char *tokens[], char *commandLine)
{
    char *token [MAX_TOKENS];
    char *workCommandLine = malloc(sizeof(commandLine) + 1);
    int i;
    int j;

    for (i = 0; i < MAX_TOKENS; i ++)
    {
        tokens [i] = "";
    }
    strcpy (workCommandLine, &commandLine);
    i = 0;
    if ((token [i] = strtok (workCommandLine, " ")) != NULL)
    {
        i ++;
        while ((token [i] = strtok (NULL, " ")) != NULL)
        {
            i ++;
        }
    }
    j = i;

    for (i = 0; i < j; i ++)
    {
        tokens [i] = token [i];
    }

    //delete [] workCommandLine;
    free(workCommandLine);

    return;
}