#include "helpers.h"


int main (int argc, char *argv []){

    // opening files, in which prompt and terminator are saved
    FILE *f = fopen("shell_name.txt", "r");
    FILE *t = fopen("shell_terminator.txt", "r");

    if (f)
        fgets(prompt, sizeof(prompt), f);
    else
        printf("Failed to open file for shell name!");

    if (t)
        fgets(terminator, sizeof(terminator), t);
    else
        printf("Failed to open file for terminator!");

    // Loop
    ExecuteShellProgram();

    // cleaning up exiting with success
    fclose(f);
    fclose(t);



    return EXIT_SUCCESS;
}
