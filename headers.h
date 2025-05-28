#ifndef HEADERS_H
#define HEADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <errno.h>
#include<ctype.h>
#include <grp.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_NUM_ARGS 64
#define MAX_LOG_SIZE 15
#define MAX_INPUT_LENGTH 256  
#define LOG_FILE "command_log.txt"
// Global variables (declarations)
extern char prev_dir[MAX_COMMAND_LENGTH];
extern char home_dir[MAX_COMMAND_LENGTH];
extern char hostname[MAX_COMMAND_LENGTH];
extern char prev_dir_1[MAX_COMMAND_LENGTH];
extern char user[1000];

#endif // HEADERS_H
