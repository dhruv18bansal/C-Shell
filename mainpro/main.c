#include "headers.h"
#include "log.h"
#include "commands.h"
#include"signal.h"
// #define MAX_PROCESSES 1024
char prev_dir[MAX_COMMAND_LENGTH] = "";
char home_dir[MAX_COMMAND_LENGTH] = "";
char hostname[MAX_COMMAND_LENGTH] = "";
char prev_dir_1[MAX_COMMAND_LENGTH];
// typedef struct Process {
//     char command_name[256];
//     pid_t pid;
//     int running; // 1 = running, 0 = stopped
// } Process;
Process process_list[MAX_PROCESSES];
int process_count = 0;
pid_t foreground_pid = -1; // Track the foreground process PID
int signal_sent_by_ping = 0;
char user[1000];
int flag_lala;
int file_fd; int stdout_fd;
struct termios original_termios;
int running = 1;
void handle_background_process(int sig);
void parse_command(char *command, char **args);
void handle_commands(char *input, char log[MAX_LOG_SIZE][MAX_COMMAND_LENGTH], int *log_count) {
    if(input[strlen(input)-1]=='|')
    {
        printf("WRONG SHELL COMMAND\n");
        return;
    }
    char *commands[MAX_COMMAND_LENGTH];
    int num_commands = 0;
    char *cmd = strtok(input, "|");
   
    // Split input into individual commands
    while (cmd != NULL && num_commands < MAX_CMDS) {
        
        commands[num_commands++] = cmd;
        cmd = strtok(NULL, "|");
    }
    // printf("%d\n",num_commands);
    int fd[2];
    int input_fd = -1;
    
    for (int i = 0; i < num_commands; i++) {

        char lalala[2000];
        strcpy(lalala,commands[i]);
    char* new=strtok(lalala," ");
    if (strcmp(new,"hop")==0 |strcmp(new,"exit")==0 ) {
            check_for_io(commands[i], -1,-1, log, log_count ,0,commands);
            continue;  
        }

if (i < num_commands - 1) { 
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
}

if (fork() == 0) {
    if (i < num_commands - 1) {  
        close(fd[0]); // Close unused read end of pipe
        dup2(fd[1], STDOUT_FILENO); // Redirect stdout to pipe
        close(fd[1]); // Close write end after dup2
    }
    if (input_fd != -1) { 
        dup2(input_fd, STDIN_FILENO); // Redirect stdin to pipe
        close(input_fd); // Close old input_fd
    }
    // execute_command(commands[i], input_fd, -1,log,log_count);
        
        if(strchr(commands[i],'&')!=NULL)
        {  
            char* input = commands[i];
             char *foreground_command;
            char *background_command = strsep(&input, "&");

    // Run the first command as a background process
           check_for_io(background_command,input_fd, -1, log,log_count,1,commands);

    if (input != NULL && strlen(input) > 0) {
        foreground_command = input;
        check_for_io(foreground_command,input_fd, -1, log , log_count, 0,commands);
    }

        }
        else
       {
        check_for_io(commands[i],input_fd, -1, log,log_count,0,commands);
        
       }
    exit(EXIT_SUCCESS); // Ensure child exits after command execution
} else { 
    if (i < num_commands - 1) {  
        
        close(fd[1]); // Close unused write end of pipe
    }
    if (input_fd != -1) { 
        // printf("is it coming here \n");
        close(input_fd); // Close old input_fd
    }
    input_fd = fd[0];
    //  printf("is it coming here %d\n",input_fd); // Save read end of pipe for next command
     int is_background = (strchr(commands[i], '&') != NULL);
    if (!is_background) {
        // Wait for foreground process only
        wait(NULL);
    } // Wait for child process to finish
}

    }
}
int main() {
     char command[MAX_COMMAND_LENGTH];
     char *commands[1000];
     signal(SIGINT, handle_sigint);
    //  signal(SIGTSTP, handle_sigstp);
    //  signal(SIGCHLD, handle_background_process);
    char log[MAX_LOG_SIZE][MAX_COMMAND_LENGTH];
    int log_count = 0;

    // Set the current directory as the home directory
    if (getcwd(home_dir, sizeof(home_dir)) == NULL) {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }
    strcpy(prev_dir,home_dir);
    strcpy(prev_dir_1,home_dir);

    // Get the system's hostname
    if (gethostname(hostname, sizeof(hostname)) == -1) {
        perror("gethostname");
        exit(EXIT_FAILURE);
    }
    struct passwd *pw = getpwuid(getuid());
    strcpy(user, pw->pw_name);
    // Initialize the shell and load previous logs
    load_log(log, &log_count);
    flag_lala=1;
    while (1) { int num_commands = 0;
    if(flag_lala==1)
        {shell_prompt();}
        flag_lala=1;
        // Read the command from the user
        if (fgets(command, sizeof(command), stdin) == NULL) {
           handle_ctrl_d();
        }
        command[strcspn(command, "\n")] = '\0';
        if (strstr(command, "log") == NULL) {
        add_to_log(log, &log_count, command);
        save_log(log, log_count);}
           int status;
    pid_t pid;

    // Wait for any child process (non-blocking)
   while ((pid = waitpid(-1, &status, WNOHANG )) > 0) {
     for (int i = 0; i < process_count; i++)
     { if (process_list[i].pid == pid)
     { if (signal_sent_by_ping) {
                    signal_sent_by_ping = 0; // Reset flag
                    break; // Don't print duplicate messages
                }
        if (WIFEXITED(status)) {
            printf("Process %d exited normally (success)\n", pid);
             remove_background_process(pid);
        } 
        // else if (WIFSIGNALED(status)) {
        //     printf("Process %d exited abnormally (signal %d)\n", pid, WTERMSIG(status));
        // }
        else if (WIFSTOPPED(status)) {
                    // Process is stopped
                    process_list[i].running = 0;
                    printf("Process (PID %d) stopped\n", pid);
                } else if (WIFCONTINUED(status)) { 
                    // Process has resumed
                    if( process_list[i].running = 0)
                    {process_list[i].running = 1; printf("%d",process_list[i].running);
                    printf("Process (PID %d) continued\n", pid);}
                }
        // shell_prompt();
        break;
    }
    
     }
}
        tokenize_and_execute(command,log,&log_count,commands);
        //  printf("Initial command: %s\n", command);
        // char *cmd = strtok(command, ";");
        //  while (cmd != NULL && num_commands < 1000) {
        // Create and copy the current command to new_command
    //     char *new_command = malloc(strlen(cmd) + 1);
    //     if (new_command == NULL) {
    //         perror("malloc");
    //         exit(EXIT_FAILURE);
    //     }
    //     strcpy(new_command, cmd);

    //     // Trim whitespace from new_command
    //     trim_whitespace(new_command);

    //     // Store the trimmed command in the commands array
    //     commands[num_commands++] = new_command;

    //     // Get the next command
    //     cmd = strtok(NULL, ";");
    // }
    //     // while (cmd != NULL) {
    //     //     // Trim whitespace and execute the command
    //     //     char trimmed_cmd[MAX_COMMAND_LENGTH];
    //     //     strncpy(trimmed_cmd, cmd, MAX_COMMAND_LENGTH - 1);
    //     //     trimmed_cmd[MAX_COMMAND_LENGTH - 1] = '\0';
    //     //     trim_whitespace(trimmed_cmd);
    //     //     execute_command(trimmed_cmd, log, &log_count);

    //     //     cmd = strtok(NULL, ";");
    //     // }
    //     //  printf("%d\n",num_commands);
    // // Iterate through the commands array and execute each command
    // for (int i = 0; i < num_commands; i++) {
    //     // printf("Processing command: '%s'\n", commands[i]);
    //     if(strchr(commands[i],'&')!=NULL)
    //     {
    //         char* input = commands[i];
    //          char *foreground_command;
    //         char *background_command = strsep(&input, "&");

    // // Run the first command as a background process
    //         execute_command(background_command, log,&log_count,1);

    // if (input != NULL && strlen(input) > 0) {
    //     foreground_command = input;
    //     execute_command(foreground_command, log , &log_count, 0);
    // }

    //     }
    //     else
    //    { execute_command(commands[i],log,&log_count,0);}

    //     // Free the allocated memory for the command
    //     free(commands[i]);
    // }
       
    }

    return 0;
}
void ek_aur_function(char* command,char log[MAX_LOG_SIZE][MAX_COMMAND_LENGTH], int *log_count, char **commands)
{

}

 int countpipes(const char *string) {
    int count = 0;
    int length = strlen(string);

    for (int i = 0; i < length; i++) {
        if (string[i] =='|') {
            count++;
        }
    }

    return count;
} 
void tokenize_and_execute(char* command,char log[MAX_LOG_SIZE][MAX_COMMAND_LENGTH], int *log_count, char **commands)
{ int num_commands=0;
    char *cmd = strtok(command, ";");
         while (cmd != NULL && num_commands < 1000) {
        // Create and copy the current command to new_command
        char *new_command = malloc(strlen(cmd) + 1);
        if (new_command == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        strcpy(new_command, cmd);

        // Trim whitespace from new_command
        trim_whitespace(new_command);

        // Store the trimmed command in the commands array
        commands[num_commands++] = new_command;

        // Get the next command
        cmd = strtok(NULL, ";");
    }
      
    for (int i = 0; i < num_commands; i++) {
        char new[2024];
        strcpy(new,commands[i]);
        char * pp=strtok(new," ");
        if (strcmp(pp, "activities") == 0) {
            print_background_process_list();
           continue;
        }
    else if (strcmp(pp, "ping") == 0) {
            handle_ping_command(commands[i]+4); // Skip "ping " prefix
           continue;
        }
         else if (strncmp(pp, "fg", 2) == 0) {
            handle_fg_command(commands[i] + 3); // Skip "fg " prefix
            continue;
        }
         else if (strcmp(pp, "bg") == 0) {
            handle_bg_command(command + 3); // Skip "bg " prefix
            continue;
        }
        int pipes=countpipes(commands[i]);
        if(pipes!=0)
       { handle_commands(commands[i],log, log_count);}
        else{
              if(strchr(commands[i],'&')!=NULL)
        {  
            char* input = commands[i];
             char *foreground_command;
            char *background_command = strsep(&input, "&");

    // Run the first command as a background process
           check_for_io(background_command,-1, -1, log,log_count,1,commands);

    if (input != NULL && strlen(input) > 0) {
        foreground_command = input;
        check_for_io(foreground_command,-1, -1, log , log_count, 0,commands);
    }

        }
        else
       { handle_commands(commands[i], log,log_count);
        
       }
        }
       if(commands[i]!=NULL)
        {free(commands[i]);commands[i]=NULL;}
    }
       
}
// void handle_background_process(int sig) {
 
// }
