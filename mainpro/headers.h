#ifndef HEADERS_H
#define HEADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <limits.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <time.h>
#include <stdbool.h>
#include <grp.h>
#include <ctype.h>
#include <sys/time.h>
#include <fcntl.h>
#include <signal.h>
#include <termios.h>
#include<netdb.h>
#include<arpa/inet.h>
#include <curl/curl.h>
#define MAX_COMMAND_LENGTH 1024
#define MAX_CMDS 100
#define MAX_NUM_ARGS 64
#define MAX_PROCESSES 1024
#define MAX_LOG_SIZE 15
#define MAX_INPUT_LENGTH 256  
#define LOG_FILE "command_log.txt"
// Global variables (declarations)
extern char prev_dir[MAX_COMMAND_LENGTH];
extern char prev_dir_1[MAX_COMMAND_LENGTH];
extern char home_dir[MAX_COMMAND_LENGTH];
extern char hostname[MAX_COMMAND_LENGTH];
extern char user[1000];
extern int flag_lala;
extern struct termios original_termios;
extern int running ;
typedef struct Process {
    char command_name[256];
    pid_t pid;
    int running; // 1 = running, 0 = stopped
} Process;
extern Process process_list[MAX_PROCESSES];
extern int process_count ;
extern int signal_sent_by_ping;
extern pid_t foreground_pid ;
 extern int file_fd; extern int stdout_fd;
 void add_background_process(char *command, pid_t pid);
 void remove_background_process(pid_t pid);
 void print_background_process_list();
void tokenize_and_execute(char* command,char log[MAX_LOG_SIZE][MAX_COMMAND_LENGTH], int *log_count, char **commands);
void handle_ping_command(char *command);
void handle_fg_command(char *command);
void handle_bg_command(char *command);
#endif // HEADERS_H
