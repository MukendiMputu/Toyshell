# Toyshell: Assignment from CS 'Operating Systems'

## PART 1

Your toyshell program must provide services to support the following commands and features:

Note: The commands are shown in uppercase font for emphasis only, you don’t have to program them that way. Similarly, the parameters to the commands are enclosed between the symbols < and > for emphasis, but the symbols are not actually part of the parameters and should not be programmed that way.

a. When toyshell is first loaded, it should display the following command prompt:

    toyshell[1]>

The command prompt consists of three parts: (1) the first part, toyshell, is the shell name, (2), the second part, [1], is the command counter, and (3) the third part, >, is the terminator. The command counter keeps track of and displays the number of the next command to be entered on the command line in the current session (i.e., since toyshell was last loaded). For example, the command prompt shown above indicates the next command to be entered on the command line in this session will be the first. Similarly, the command prompt

    toyshell[27]>

indicates the next command to be entered in the current session will be the twenty-seventh. If the user presses enter at the command prompt, without actually entering anything on the command line, the command counter should not change. In all other cases, whether the command entered is valid or not, the command counter should be incremented.

b. STOP: Terminates execution of the current toyshell session.

c. SETSHELLNAME <shell_name>: Sets the shell name in the toyshell command prompt to shell_name. The new shell name should be persistent. That is, it should be used for the remainder of the current session and any subsequent sessions.

d. SETTERMINATOR <terminator>: Sets the terminator in the toyshell command prompt to terminator. The new terminator should be persistent. That is, it should be used for the remainder of the current session and any subsequent sessions.

e. HISTORY: Lists the commands that have been entered in the current session (i.e., since the last time toyshell was run). The commands should be stored in a structure of size 10 (i.e., it can hold up to ten commands). Commands should be added to the structure in sequence until it becomes full. Any command entered at the command prompt when the structure is full will require that the oldest command be removed from the structure before it can be added to the structure.

f. !  <n>: Re-executes a command stored in the history structure. For example, the command ! 6 will cause the command in the 6-th position in the history structure to be executed again. The re-executed command should be added to the history structure as it was originally issued on the command line.

g. NEWNAME <new_name> | <new_name> <old_name>: Manages a structure containing command aliases. The aliases should be stored in a structure of size 10 (i.e., it can hold up to ten aliases). The first option deletes a previously defined alias. The second option defines an alias for another command. For example, the command NEWNAME mymove deletes the alias for mymove, and the command NEWNAME mycopy cp defines mycopy as the alias for the cp command. If an alias for a command already exists, then the new alias replaces the old alias.

h. Alias Substitution: Every command entered on the command line should be checked to determine whether it is an alias by scanning the alias structure. When an alias is detected in a command, the old name should be substituted into the command before the command is executed. For example, assume the following aliases are defined as shown below:

    aa ls –l
    bb grep toyshell
    cc grep .cpp
    dd aa | bb
    ee dd

Then, for the following sequence of commands at the command prompt:

    aa
    aa | bb
    aa | bb | cc
    dd | cc
    ee

alias substitutions should result in the following sequence of commands being executed:

    ls –l
    ls –l | grep toyshell 
    ls –l | grep toyshell | grep .cpp
    ls –l | grep toyshell | grep .cpp
    ls –l | grep toyshell

i. NEWNAMES: Outputs all the aliases contained in the alias structure. Each command/alias pair should be shown on one line. For example, the possible aliases for a few commands are shown below:

mymove mv
mycopy cp
chkshell ls –l | grep toyshell.cpp

j. SAVENEWNAMES <file_name>: Stores all currently defined aliases in a file whose name is file_name.

k. READNEWNAMES <file_name>: Reads all aliases stored in the file whose name is file_name and adds them to the aliases defined in the current session. If an alias in file_name already exists in the alias structure (i.e., it is a duplicate), it should be ignored.

l. <Linux_command>: Executes the Linux command Linux_command, corresponding to any valid Linux command. One approach to implementing this is to use the system function. If the first token on a command line is not a built-in toyshell command, assume that it is a Linux command.

m. Error handling: Your approach should effectively identify and recover from errors. For example, bad input and/or the inability to execute a command should not cause your toyshell to crash. To get credit for this component of the assignment, you must be able to demonstrate that you can handle at least ten unique errors.

Note: You must handle all the built-in commands with exactly the same syntax as shown above. Thus, an important part of the toyshell program will be to parse commands entered at the command prompt to break them down into their component parts. Once a command has been parsed, the component parts can be checked to ensure that a valid command has been entered and that it adheres to the required syntax. One approach to effective parsing is to use the C-string strtok function (to be covered in detail in the first lab).
