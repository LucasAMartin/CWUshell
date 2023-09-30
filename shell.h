#ifndef SHELL_H
#define SHELL_H

void printPrompt1(void);
void printPrompt2(void);

char *readCommand(void);
void parseCommand(char *command, char **parsedCommands);
void executeCommand(char **parsedCommands);

#endif