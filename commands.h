#ifndef COMMANDS_H
#define COMMANDS_H

#include "headers.h"

void shell_prompt();
void change_directory(char *path);
void execute_command(char *command, char log[MAX_LOG_SIZE][MAX_COMMAND_LENGTH], int *log_count);
char *trim_whitespace(char *str);

#endif // COMMANDS_H
