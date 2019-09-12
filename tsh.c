#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void StopTheShell();
void TokenizeCommandLine (char *tokens[], char *commandLine);
int ExecuteShellProgram(char *prompt, int counter, char *terminator);

#define MAX_TOKENS 1024

int main (int argc, char *argv []){
    
    // initialize counter, prompt and terminated 
    int counter = 1;
    char *prompt = (char *) malloc(sizeof(char)*256);
    char *terminator =  (char *) malloc(sizeof(char)*256); 

    // get and/or set the terminator 
    prompt = "toyshell";
    terminator = ">";
    
    // Loop 
    ExecuteShellProgram(prompt, counter, terminator);
    
    // Free allocated memory above 
    free(prompt);
    free(terminator);

    return 0;
}

/* Loop until the program is terminated */
int ExecuteShellProgram(char *prompt, int counter, char *terminator) {
    
    // ToDo: display shell name, counter and prompt 
    //       fetch the commandLine
    //       tokenize the CL
    //       execute the command
   
    char *rawCommand = (char *) malloc(sizeof(char)*256);
    char *commandTokens = (char *) malloc(sizeof(char)*MAX_TOKENS);
    size_t lineBuff = 0;
    
    // loop until the stop command is entered
    while (1)
    {
        //print the prompt and wait for user to enter a command
        printf("%s[%i]%s ", prompt, counter, terminator);
        if(getline(&rawCommand, &lineBuff, stdin) != -1){
            
            /* TokenizeCommandLine(&commandTokens, rawCommand);
            if(&commandTokens[0] == "stop")
                StopTheShell(); */

            if(*commandTokens != "\n")
                counter++;

        } else {
            //perror("scanf");
        }

    }
    
    free(rawCommand);
    free(commandTokens);

    return 0;
}

void StopTheShell (){
    return;
}


// Helper function to disect the command line
void TokenizeCommandLine (char *tokens[], char *commandLine){
    
    char *token [MAX_TOKENS];
    char *workCommandLine = (char*) malloc(sizeof(commandLine) + 1);
    int i;
    int j;

    for (i = 0; i < MAX_TOKENS; i++)
        tokens[i] = "";
    
    strcpy (workCommandLine, commandLine);
    i = 0;
    if ((token[i] = strtok(workCommandLine, " ")) != NULL){
        i++;
        while ((token[i] = strtok(NULL, " ")) != NULL)
            i++;
    }
    j = i;

    for (i = 0; i < j; i++)
        tokens[i] = token[i];

    free(workCommandLine);

    return;
}