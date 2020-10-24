#ifndef HEPERS_H
#define HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <ctype.h>

extern char prompt[128];
extern char terminator[128];
extern char aliases_buffer[10][2];


void StopTheShell();
int IsAlias(char *token);
int TokenizeCommandLine (char * tokens[], char *commandLine);
int ExecuteShellProgram();
void ProcessCommand (char * tokens[], int tokenCount);
int SetShellName(char *shell_name);
int SetShellTerminator(char *shell_terminator);
int IsBuiltinCommand (char * tokens[], int tokenCount);
void TrimCommandLine(char *command);
void SaveInHistory(char *command);
void FetchingBang(char *command);

#endif