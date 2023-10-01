#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/utsname.h>

/* 
RUN COMMAND

gcc -o shell main.c
./shell
*/

// Function prototypes
char *readInput(void);
void parseInput(char *command, char **parsedCommands);
void executeInput(char *command, char **parsedCommands);
void exitCommand(char **parsedCommands);
void fileinfoCommand(char **parsedCommands);
void osinfoCommand(char **parsedCommands);
void promptCommand(char **parsedCommands);

// Global variable for the command prompt
char *prompt = NULL;

int main()
{
    char *command;
    char *parsedCommands[30];
    prompt = strdup("cwushell>");

    while(1) {
        fprintf(stderr, "%s ", prompt);

        command = readInput();

        parseInput(command, parsedCommands);
        executeInput(command, parsedCommands);

        free(command);
    }

    return 0;
}

// Function to read input from the user
char *readInput(void)
{
    char input[1024];
    char *output= NULL;

    // Read a line from stdin
    if(fgets(input, 1024, stdin))
    {
        int inputLength = strlen(input);

        // Allocate memory for the output string
        output = malloc(inputLength+1);
        if(!output)
        {
            fprintf(stderr, "error: failed to alloc buffer: %s\n", strerror(errno));
            return NULL;
        }

        // Copy the input to the output string
        strcpy(output, input);
    }
    return output;
}

// Function to parse the input command into tokens
void parseInput(char *command, char **parsedCommands){
    char *token;
    char *commandCopy = strdup(command);
    char *rest = commandCopy;
    int i = 0;

    // Tokenize the command string on spaces
    while ((token = strtok_r(rest, " ", &rest))) {
        // Remove newline character if it exists
        token[strcspn(token, "\n")] = '\0';
        parsedCommands[i] = token;
        i++;
    }
    parsedCommands[i] = NULL; // Mark the end of the tokens
}

// Function to execute the appropriate function based on the input command
void executeInput(char *command, char **parsedCommands) {
    if (strcmp(parsedCommands[0], "exit") == 0) {
        exitCommand(parsedCommands);
    } else if (strcmp(parsedCommands[0], "prompt") == 0) {
        promptCommand(parsedCommands);
    } else if (strcmp(parsedCommands[0], "fileinfo") == 0) {
        fileinfoCommand(parsedCommands);
    } else if (strcmp(parsedCommands[0], "osinfo") == 0) {
        osinfoCommand(parsedCommands);
    } else {
        // If no known command is found, execute the command using system()
        system(command);
    }
}


void exitCommand(char **parsedCommands){
    // Check for help switch
    if (parsedCommands[1] != NULL && (strcmp(parsedCommands[1], "-h") == 0 || strcmp(parsedCommands[1], "--help") == 0)) {
        printf("Usage: exit [n]\n");
        printf("Exit supports an optional exit value:\n");
        printf("n: Exit value of the shell's execution\n");
        fflush(stdout);
        return;
    }
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
    // Check for help switch
    if (parsedCommands[1] != NULL && (strcmp(parsedCommands[1], "-h") == 0 || strcmp(parsedCommands[1], "--help") == 0)) {
        printf("Usage: prompt [new_prompt]\n");
        printf("By default prompt will be reset to cwushell>\n");
        printf("Prompt supports an optional prompt value:\n");
        printf("new_prompt: New prompt to be used\n");
        fflush(stdout);
        return;
    }
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

void fileinfoCommand(char **parsedCommands) {
    struct stat fileinfo;
    char *filename = parsedCommands[1];

    // Check for help switch
    if (parsedCommands[1] != NULL && (strcmp(parsedCommands[1], "-h") == 0 || strcmp(parsedCommands[1], "--help") == 0)) {
        printf("Usage: fileinfo [-switch] filename\n");
        printf("fileinfo supports the following switches:\n");
        printf("-i: Print the inode number of the file\n");
        printf("-t: Print the type of the file\n");
        printf("-m: Print the last modification date of the file\n");
        fflush(stdout);
        return;
    }

    // Check if filename is provided
    if (filename == NULL) {
        printf("Please provide a filename.\n");
        return;
    }

    // Get file info
    if (stat(filename, &fileinfo) != 0) {
        perror("Error getting file info");
        return;
    }

    int i_used = 0, t_used = 0, m_used = 0;

    if (parsedCommands[2] == NULL){
        i_used = 1, t_used = 1, m_used = 1;
    }

    // Iterate over the parsedCommands array
    for (int i = 2; parsedCommands[i] != NULL; i++) {
        // If the argument starts with '-', it's a switch
        if (parsedCommands[i][0] == '-') {
            // If the argument is just '-', it's an error
            if (parsedCommands[i][1] == '\0') {
                printf("Unknown switch: -\nTry 'fileinfo --help' for more information.\n");
                fflush(stdout);
                return;
            }
            // Iterate over the characters in the switch
            for (int j = 1; parsedCommands[i][j] != '\0'; j++) {
                switch (parsedCommands[i][j]) {
                    case 'i':
                        i_used = 1;
                        break;
                    case 't':
                        t_used = 1;
                        break;
                    case 'm':
                        m_used = 1;
                        break;
                    default:
                        printf("Unknown switch: -%c\nTry 'fileinfo --help' for more information.\n", parsedCommands[i][j]);
                        fflush(stdout);
                        return; // Stop execution if an unknown switch is encountered
                }
            }
        } 
    }

    // Print the information for used switches
    if (i_used) printf("Inode number: %ld\n", (long)fileinfo.st_ino);
    if (t_used) {
        printf("File type: ");
        if (S_ISREG(fileinfo.st_mode)) {
            printf("regular file\n");
        } else if (S_ISDIR(fileinfo.st_mode)) {
            printf("directory\n");
        } else if (S_ISLNK(fileinfo.st_mode)) {
         printf("symbolic link\n");
        } else {
          printf("other\n");
        }
    }
    if (m_used) {
        char date[20];
        strftime(date, 20, "%Y-%m-%d %H:%M:%S", localtime(&(fileinfo.st_mtime)));
        printf("Last modification date: %s\n", date);
    }
    
}

void osinfoCommand(char **parsedCommands) {
    struct utsname osinfo;
    uname(&osinfo);

    // Check for help switch
    if (parsedCommands[1] == NULL || parsedCommands[1] != NULL && (strcmp(parsedCommands[1], "-h") == 0 || strcmp(parsedCommands[1], "--help") == 0)) {
        printf("Usage: osinfo -switch\n");
        printf("osinfo supports the following switches:\n");
        printf("-s: Print the operating system name\n");
        printf("-v: Print the operating system version\n");
        printf("-a: Print the computer architecture\n");
        fflush(stdout);
        return;
    }

    int s_used = 0, v_used = 0, a_used = 0;

    // Iterate over the parsedCommands array
    for (int i = 1; parsedCommands[i] != NULL; i++) {
        // If the argument starts with '-', it's a switch
        if (parsedCommands[i][0] == '-') {
            // If the argument is just '-', it's an error
            if (parsedCommands[i][1] == '\0') {
                printf("Unknown switch: -\nTry 'osinfo --help' for more information.\n");
                fflush(stdout);
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
                        printf("Unknown switch: -%c\nTry 'osinfo --help' for more information.\n", parsedCommands[i][j]);
                        fflush(stdout);
                        return; // Stop execution if an unknown switch is encountered
                }
            }
        } else {
            // Arguments that don't start with '-' are errors
            printf("Unknown argument: %s\nTry 'osinfo --help' for more information.\n", parsedCommands[i]);
            fflush(stdout);
            return;
        }
    }

    // Print the information for used switches
    if (s_used) printf("Operating System: %s\n", osinfo.sysname);
    if (v_used) printf("OS Version: %s\n", osinfo.release);
    if (a_used) printf("Computer Architecture: %s\n", osinfo.machine);
}