#ifndef NEONATE_H
#define NEONATE_H

#include "headers.h"

void handleError(const char *msg);
void resetTerminalMode();
void setRawMode();
int isKeyPressed();
pid_t get_recent_pid();
void sigint_handler(int sig);
#endif // COMMANDS_H