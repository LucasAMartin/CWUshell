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

char *prompt = NULL;

int main()
{
    char *command;
    char *parsedCommands[30];
    prompt = strdup("cwushell>");


    while(1) {
        fprintf(stderr, "%s ", prompt);

        command = readCommand();

        parseCommand(command, parsedCommands);
        executeCommand(command, parsedCommands);

        free(command);
    }

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
    char *commandCopy = strdup(command);
    char *rest = commandCopy;
    int i = 0;

    while ((token = strtok_r(rest, " ", &rest))) {
        // Remove newline character if it exists
        token[strcspn(token, "\n")] = '\0';
        parsedCommands[i] = token;
        i++;
    }
    parsedCommands[i] = NULL; // Mark the end of the tokens
    free(commandCopy);
}

void executeCommand(char *command, char **parsedCommands) {
    if (strcmp(parsedCommands[0], "exit") == 0) {
        exitCommand(parsedCommands);
    } else if (strcmp(parsedCommands[0], "prompt") == 0) {
        promptCommand(parsedCommands);
    } else if (strcmp(parsedCommands[0], "fileinfo") == 0) {
        printf("fileinfo\n");
    } else if (strcmp(parsedCommands[0], "osinfo") == 0) {
        printf("osinfo\n");
    } else {
        system(command);
    }
} 

void exitCommand(char **parsedCommands){
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
} 

void promptCommand(char **parsedCommands){
    if (parsedCommands[1] != NULL) {
        char *newPrompt = malloc(1024); // Allocate memory for newPrompt
        strcpy(newPrompt, parsedCommands[1]); // Copy the first part of the prompt

        // Concatenate all other parts of the prompt
        for (int i = 2; parsedCommands[i] != NULL; i++) {
            strcat(newPrompt, " ");
            strcat(newPrompt, parsedCommands[i]);
        }

        free(prompt); // Free the old prompt
        prompt = newPrompt;
    } else {
        free(prompt); // Free the old prompt
        prompt = strdup("cwushell>"); // Allocate new memory for "cwushell"
    } 
}



