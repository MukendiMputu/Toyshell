#include "helpers.h"


#define MAX_COMMAND_IN_HISTORY 10
#define MAX_COMMAND_LENGTH 512

// initializing counter, prompt and terminated 
int counter = 1, alias_count = 0, bg_exec = 0;
FILE *shell_name;
FILE *shell_terminator;
char history_buffer[MAX_COMMAND_IN_HISTORY][MAX_COMMAND_LENGTH];
char *environ[] = {(char*)0 };

/* struct history_entry{
    int idx;
    char entry[MAX_COMMAND_LENGTH];
} history_buffer[10] = {1, "", 2, "", 3, "", 4, "", 5, "", 6, "", 7, "", 8, "", 9, "", 10, ""}; */

struct alias{
    char *name;
    char *substitute;
} aliases[10] = {"aa", "", "bb", "", "cc", "", "dd", "", "ee", "", "ff", "", "gg", "", "hh", "", "ii", "", "jj", ""};


// Loop until the program is terminated
int ExecuteShellProgram() {
    
    int tokenNumb = 0;                                          // will be use to count to CL tokens
    size_t lineBuff = 256;                                      // size of a raw command entered by the user
    char *rawCommand = (char *) malloc( MAX_COMMAND_LENGTH);
    
    char* commandTokens[] = {"", "", "", "", "", "", "", ""};
    
    /* Loop until the program is terminated */
    while (1){

        //print out the prompt and wait... if user entered a command, then tokenize and process in
        printf("%s[%i]%s ", prompt, counter, terminator);
        
        if(getline(&rawCommand, &lineBuff, stdin) != -1){
            
            if((rawCommand != NULL) && (rawCommand[0] != '\n')){
                SaveInHistory(rawCommand); //put command in history_buffer
                counter++; 
            }

            if(rawCommand[0] == ' ')
                TrimCommandLine(rawCommand);

            FindAndIgnore$(rawCommand);
            Background(rawCommand); // exec in bg?
            while(FetchingBang(rawCommand))// '!' needs to be processed before tokenizing
                ; 

            tokenNumb = TokenizeCommandLine(commandTokens, rawCommand);
            ProcessCommand(commandTokens, tokenNumb);

        } else {
            perror(prompt);
        }
    }
    free(rawCommand);

    return EXIT_SUCCESS;
}

/* terminates and exits the shell program properly
 * @param
 * @return
 */
void StopTheShell (){
    printf("Exiting shell program ...\n");
    exit(EXIT_SUCCESS);
}

/* looks up for command aliases in an array
 * @param token alias to look up for
 * @return 1 when found and 0 when not contained
 */ 
int IsAlias(char *token){
    /* for (int i = 0; i < sizeof(history_buffer); i++)
    {
        if(!strcmp(token, *history_buffer)){
            return EXIST_FAILURE;
        }
    } */
    
    return EXIT_SUCCESS;
}

/* sets the prompt persistently to
 * @param shellName new prompt to be shown
 * @return 1 when failed and 0 when successed
*/
int SetShellName(char *shellName){
    shell_name = fopen("shell_name.txt","w+");
    strncpy(prompt, shellName, strlen(shellName));

    if (shell_name)
        fputs(prompt, shell_name);
    else{
        printf("Failed to save shell name to a file!");
        return EXIT_FAILURE;
    }
    fclose(shell_name);
    return EXIT_SUCCESS;
}

/* sets the terminator persistently to
 * @param shellTerminator new terminator to be shown
 * @return 1 when failed and 0 when successed
*/
int SetShellTerminator(char *shellTerminator){
    shell_terminator = fopen("shell_terminator.txt","w+");
    strncpy(terminator, shellTerminator, strlen(shellTerminator));

    if (shell_terminator)
        fputs(terminator, shell_terminator);
    else{
        printf("Failed to save shell terminator to a file!");
        return EXIT_FAILURE;
    }
    fclose(shell_terminator);
    return EXIT_SUCCESS;
}

/* 
 * @param
 * @return
 */
int TokenizeCommandLine( char *tokens[], char *buff){
	int token_count = 0;
	int in_token = 0;
	int num_chars = strnlen(buff, 256);
    char *workCommand = (char *) malloc(sizeof(buff));

    strncpy(workCommand, buff, num_chars);

    for (int i = 0; i < num_chars; i++) {
		switch (workCommand[i]) {
		// Handle token delimiters (ends):
		case ' ':
		case '\t':
		case '\n':
			workCommand[i] = '\0';
			in_token = 0;
			break;

		// Handle other characters (may be start)
		default:
			if (!in_token) {
				tokens[token_count] = &workCommand[i];
				token_count++;
				in_token = 1;
			}
		}
	}
	tokens[token_count] = NULL;

    return token_count;
}

/* processes the command with the given param
 * @param pointer to array of command and param tokens
 * @return
*/
void ProcessCommand (char * tokens[], int tokenCount){
    int status;

    if (!IsBuiltinCommand(tokens, tokenCount))
        return;
    
    *environ = getenv("PATH");  // fetch the value of the PATH environment variable

    __pid_t pid = fork();
    if (pid < 0){
        perror(prompt);

    }else if (pid == 0) {                           //the command is executed in the child process
        if(access(tokens[0], F_OK) == 0){           // check wether the program file exists or not
            if (execve(tokens[0], tokens, environ)){
                perror(prompt);
                exit(EXIT_FAILURE);
            }
        }
        
    }else { // meanwhile the parent process waits
        if(!bg_exec)    
            wait(&status);
    }

    return; 
}

/* identifies the customed commands
 * @param 
 * @return
 */ 
int IsBuiltinCommand (char * tokens[], int tokenCount){
    
    if (tokens[0] == NULL)
        return EXIT_FAILURE;
    
    if (!strcmp(tokens[0],"stop")){

    	if (tokenCount > 1){
       		printf ("toyshell: %s does not require any arguments.\n", tokens[0]);
    	}
    	StopTheShell();
    }

    if (!strcmp(tokens[0], "backjobs"))
    {
        if (tokenCount > 1){
       		printf ("toyshell: %s does not require any arguments.\n", tokens[0]);
    	}

    }
    

    if (!strcmp(tokens[0],"setshellname")){

    	if (tokenCount > 2){
       		printf ("toyshell: Usage %s <shell_name>.\n", tokens[0]);
            return EXIT_SUCCESS;
    	}
    	SetShellName(tokens[1]);
        return EXIT_SUCCESS;
    }

    if (!strcmp(tokens[0],"setterminator")){

    	if (tokenCount > 2 || tokenCount <= 1){
       		printf ("toyshell: Usage %s <shell_terminator>.\n", tokens[0]);
            return EXIT_SUCCESS;
    	}
    	SetShellTerminator(tokens[1]);
        return EXIT_SUCCESS;
    }

    if (!strcmp(tokens[0],"history")){

    	if (tokenCount > 2){
       		printf ("toyshell: %s does not require a parameter.\n", tokens[0]);
            return EXIT_SUCCESS;
    	}
    	for (int i = 0; i < MAX_COMMAND_IN_HISTORY; i++){
            if (*history_buffer[i] != '\0')
                printf("%i %s", i+1, history_buffer[i]);
        }
        printf("\n");
        return EXIT_SUCCESS;
    }

    if (!strcmp(tokens[0],"newname")){

    	if (tokenCount > 3){
       		printf ("toyshell: Usage %s <new_name> | <new_name> <old_name>.\n", tokens[0]);
            return EXIT_SUCCESS;
    	}
    	// ToDo 
        /**
         * for every entry in aliases_buffer
         *    if compare to token[1] positive
         *      then override corresponding entry with token[2]
         */
        for (int i = 0; i < 10; i++){
            if (tokens[1] = aliases[i].name)
                aliases[i].substitute = tokens[2];
        }
            if(alias_count <10){
                aliases[alias_count].name = tokens[1];
                aliases[alias_count].substitute = tokens[2];
            }else
            {
                printf("Alias's buffer is full. You should override one of the existing alias.\n");
            }
            
        return EXIT_SUCCESS;
    }

    if (!strcmp(tokens[0],"newnames")){

    	if (tokenCount > 2){
       		printf ("toyshell: %s does not require a parameter.\n", tokens[0]);
            return EXIT_SUCCESS;
    	}
    	// ToDo
        return EXIT_SUCCESS;
    }

    if (!strcmp(tokens[0],"savenewnames")){

    	if (tokenCount > 2){
       		printf ("toyshell: Usage %s <file_name>.\n", tokens[0]);
            return EXIT_SUCCESS;
    	}
    	// ToDo
        return EXIT_SUCCESS;
    }

    if (!strcmp(tokens[0],"readnewnames")){

    	if (tokenCount > 2){
       		printf ("toyshell: Usage %s <file_name>.\n", tokens[0]);
            return EXIT_SUCCESS;
    	}
    	// ToDo
        return EXIT_SUCCESS;
    }

    if(IsAlias(tokens[0])){

    }

    return 1;
}

/** This function uses pointer arithmetic to trim leading space characters
 *  out of command entered. As long as a character in the command is a white
 *  space, we delete it (assigning \0 to it) and increment to starting address
 *  our pointer points to by one.
 *  @param char *commandLine - rawcommand
 *  @return 
 */
void TrimCommandLine(char *commandLine){
    char *temp = commandLine;

    do{
        *temp = '\0'; // deletion
        temp += 1;    // incrementing address 
        temp++; // fetching the next character
    }while(*temp == ' ');
    memset(commandLine, 0, sizeof(commandLine));
    strncpy(commandLine, temp, sizeof(temp));

}

void SaveInHistory(char *commandLine){
    if (counter >= 10){

        // in order to simulate a stack, we'll use a temporary buffer for  
        char shift_buffer[MAX_COMMAND_IN_HISTORY-1][MAX_COMMAND_LENGTH];
        for (int i = 0 ; i < 9; i++)
            memset(history_buffer[i], 0, sizeof(history_buffer[i]));
        
        
        for(int idxShift = 0; idxShift < 9; idxShift++){
            //transfering the actual content of the history_buffer with strncpy,..
            strncpy(shift_buffer[idxShift], history_buffer[idxShift+1], sizeof(history_buffer[idxShift+1]));
            memset(history_buffer[idxShift+1], 0, sizeof(history_buffer[idxShift+1]));
        }
        //...then with memset reset the memory 
        memset(history_buffer[0], 0, sizeof(history_buffer[0]));
        
        for(int idxShift = 0; idxShift < 9; idxShift++) {

            strncpy(history_buffer[idxShift], shift_buffer[idxShift], sizeof(shift_buffer[idxShift]));
            memset(shift_buffer[idxShift], 0, sizeof(shift_buffer[idxShift]));
        }
        memset(history_buffer[9], 0, sizeof(history_buffer[9]));
        strncpy(history_buffer[9], commandLine, sizeof(commandLine)); // copying the new command entered

    } else {
        strncpy(history_buffer[counter-1], commandLine, sizeof(commandLine));
    }
}

int FetchingBang(char *commandLine){
    if (commandLine[0] == '!'){
        if (strlen(commandLine) > 5){
            printf ("toyshell: Usage ! <n> \n\t\twith n between 1 and 10.\n");
            return 0;
        }
        if (strlen(commandLine) == 2 || strlen(commandLine) == 5){
            if (sizeof(commandLine) == 5 && (commandLine[2]+commandLine[3]) >= 2){
                printf("toyshell: Enter a number between 1 and 10.\n");
                memset(commandLine, 0, sizeof(commandLine));
                
                return 0;
            }
            memset(commandLine, 0, sizeof(commandLine));
            strncpy(commandLine, history_buffer[9], sizeof(history_buffer[9]));
            return 1;
        }

        int pos = commandLine[2]-48; //in ASCII numbers start from 48
        memset(commandLine, 0, sizeof(commandLine));
        strncpy(commandLine, history_buffer[pos-1], sizeof(history_buffer[pos-1]));
        
        if(commandLine[0] == ' ')
                TrimCommandLine(commandLine);

        return 1;
    }
    return 0;
}

int FindAndIgnore$(char *commandLine){
    int i = 0;
    int found_comment = 0;
    for (; i < strlen(commandLine); i++){
        if(commandLine[i] == '$'){
            commandLine[i] = '\n';
            found_comment = 1;
            break;
        }
    }

    if(found_comment){
        for (int j = i; j < sizeof(commandLine); j++)
        commandLine[j] = '\0';
    }
    
    return i;
}

int Background(char *commandLine){
    int i = strlen(commandLine) - 2;

    for (; i > 0; i++){
        if(commandLine[i] == '-'){
            commandLine[i] = '\n';
            bg_exec = 1;
            break;
        }else
        {
            break;
        }
        
        
    }
    
    return bg_exec;
}
