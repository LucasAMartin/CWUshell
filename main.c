#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/utsname.h>
#include "shell.h"

/*
gcc -o shell main.c prompt.c
./shell
*/

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
}

void executeCommand(char *command, char **parsedCommands) {
    if (strcmp(parsedCommands[0], "exit") == 0) {
        exitCommand(parsedCommands);
    } else if (strcmp(parsedCommands[0], "prompt") == 0) {
        promptCommand(parsedCommands);
    } else if (strcmp(parsedCommands[0], "fileinfo") == 0) {
        printf("fileinfo\n");
    } else if (strcmp(parsedCommands[0], "osinfo") == 0) {
        osinfoCommand(parsedCommands);
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

void osinfoCommand(char **parsedCommands) {
    struct utsname osinfo;
    uname(&osinfo);

    int s_used = 0, v_used = 0, a_used = 0;

    // If no switch is provided, print all information
    if (parsedCommands[1] == NULL){
        s_used = 1, v_used = 1, a_used = 1;
    }

    // Iterate over the parsedCommands array
    for (int i = 1; parsedCommands[i] != NULL; i++) {
        // If the argument starts with '-', it's a switch
        if (parsedCommands[i][0] == '-') {
            // If the argument is just '-', it's an error
            if (parsedCommands[i][1] == '\0') {
                printf("Unknown switch: -\n");
                return;
            }
            // Iterate over the characters in the switch
            for (int j = 1; parsedCommands[i][j] != '\0'; j++) {
                switch (parsedCommands[i][j]) {
                    case 's':
                        s_used = 1;
                        break;
                    case 'v':
                        v_used = 1;
                        break;
                    case 'a':
                        a_used = 1;
                        break;
                    default:
                        printf("Unknown switch: -%c\n", parsedCommands[i][j]);
                        return; // Stop execution if an unknown switch is encountered
                }
            }
        } else {
            // Arguments that don't start with '-' are errors
            printf("Unknown argument: %s\n", parsedCommands[i]);
            return;
        }
    }

    // Print the information for used switches
    if (s_used) printf("Operating System: %s\n", osinfo.sysname);
    if (v_used) printf("OS Version: %s\n", osinfo.release);
    if (a_used) printf("Computer Architecture: %s\n", osinfo.machine);
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



