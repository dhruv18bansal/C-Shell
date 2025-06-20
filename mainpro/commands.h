#ifndef COMMANDS_H
#define COMMANDS_H

#include "headers.h"

void shell_prompt();
void change_directory(char *path);
void execute_command(char *command, char log[MAX_LOG_SIZE][MAX_COMMAND_LENGTH], int *log_count,int background, char **commands);
// void tokenize_and_execute(char* command);
char *trim_whitespace(char *str);
void check_for_io(char *command, int input_fd, int output_fd,char log[MAX_LOG_SIZE][MAX_COMMAND_LENGTH], int *log_count,int background, char **commands);
#endif // COMMANDS_H
