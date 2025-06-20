#ifndef ACTIVITIES_H
#define ACTIVITIES_H

#include "headers.h"


void add_background_process(char *command, pid_t pid);
void print_background_process_list();
void remove_background_process(pid_t pid);
#endif // COMMANDS_H