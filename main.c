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

    while(1) {
        print_prompt1();

        command = read_command();

        if(!command)
        {
            exit(EXIT_SUCCESS);
        }

        if(command[0] == '\0' || strcmp(command, "\n") == 0)
        {
            free(command);
            continue;
        }

        if(strcmp(command, "exit\n") == 0)
        {
            free(command);
            break;
        }

        printf("%s\n", command);

        free(command);

    }

    exit(EXIT_SUCCESS);
}

char *read_command(void)
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