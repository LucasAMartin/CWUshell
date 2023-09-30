#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "shell.h"

/*
gcc -o shell main.c prompt.c
./shell
*/

// Found on https://stackoverflow.com/questions/17271576/clear-screen-in-c-and-c-on-unix-based-system
#define clear() printf("\033[H\033[J")

int main()
{
    char *command;
    char *parsedCommands[30];

    while(1) {
        printPrompt1();

        command = readCommand();

        parseCommand(command, parsedCommands);
        executeCommand(parsedCommands);

        free(command);

    }

    free(command);
    return 0;
}

char *readCommand(void)
{
    char input[1024];
    char *output= NULL;

    if(fgets(input, 1024, stdin))
    {
        int inputLength = strlen(input);

        output = malloc(inputLength+1);
        if(!output)
        {
            fprintf(stderr, "error: failed to alloc buffer: %s\n", strerror(errno));
            return NULL;
        }

        strcpy(output, input);
    }
    return output;
}

void parseCommand(char *command, char **parsedCommands){
    char *token;
    char *rest = command;
    int i = 0;

    while ((token = strtok_r(rest, " ", &rest))) {
        parsedCommands[i] = token;
        i++;
    }
    parsedCommands[i] = NULL; // Mark the end of the tokens
}

void executeCommand(char **parsedCommands) {
    if (strcmp(parsedCommands[0], "exit") == 0) {
        if (parsedCommands[1] != NULL) {
            char *end;
            long exitStatus = strtol(parsedCommands[1], &end, 10);
            if (*end == '\0' && exitStatus >= 0 && exitStatus <= 255) {
                // It's a valid integer, so we can exit.
                exit((int)exitStatus);
            } else {
                // It's not a valid integer.
                printf("Invalid exit status: %s\n", parsedCommands[1]);
            }
        } else {
            // No exit status provided, so we exit with status 0.
            exit(0);
        }
    } else if (strcmp(parsedCommands[0], "prompt") == 0) {
        printf("prompt\n");
    } else if (strcmp(parsedCommands[0], "fileinfo") == 0) {
        printf("fileinfo\n");
    } else if (strcmp(parsedCommands[0], "osinfo") == 0) {
        printf("osinfo\n");
    } else {
        printf("system\n");
    }
}
