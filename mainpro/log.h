#ifndef LOG_H
#define LOG_H

#include "headers.h"

void load_log(char log[MAX_LOG_SIZE][MAX_COMMAND_LENGTH], int *log_count);
void save_log(char log[MAX_LOG_SIZE][MAX_COMMAND_LENGTH], int log_count);
void print_log(char log[MAX_LOG_SIZE][MAX_COMMAND_LENGTH], int log_count);
void add_to_log(char log[MAX_LOG_SIZE][MAX_COMMAND_LENGTH], int *log_count, const char *command);
void purge_log(char log[MAX_LOG_SIZE][MAX_COMMAND_LENGTH], int *log_count);

#endif // LOG_H
