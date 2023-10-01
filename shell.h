#ifndef SHELL_H
#define SHELL_H

char *readCommand(void);
void parseCommand(char *command, char **parsedCommands);
void executeCommand(char *command, char **parsedCommands);
void exitCommand(char **parsedCommands);
void fileinfoCommand(char **parsedCommands);
void osinfoCommand(char **parsedCommands);
void promptCommand(char **parsedCommands);

#endif