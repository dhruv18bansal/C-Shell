#include "headers.h"
#include "commands.h"
#include "log.h"
#include "hop.h"
#include "reveal.h"
#include "proclore.h"
#include "seek.h"
#include "man.h"
#include "neonate.h"
void check_foreground_process(pid_t pid, char *cmd_name) {
    time_t start_time, end_time;
    time(&start_time);
    int status;

    waitpid(pid, &status, 0);  // Wait for the foreground process to finish

    time(&end_time);
    int elapsed_time = (int)difftime(end_time, start_time);

    if (elapsed_time > 2) { printf("coming");
        // printf("<JohnDoe@SYS:~ %s : %ds>\n", cmd_name, elapsed_time);
         char home[1000];
    getcwd(home, sizeof(home));
    int flag = 0;
    char changehome[1000] = "~";
    int i;

    if (strlen(home_dir) <= strlen(home))
        for (i = 0; home_dir[i] != '\0'; i++) {
            if (home_dir[i] != home[i]) {
                flag = 1;
                break;
            }
        }
    else
        flag = 1;

    if (flag == 0) { 
        int j, k;
        for (j = i, k = 1; home[j] != '\0'; j++, k++)
            changehome[k] = home[j];
      printf("\033[1;33m%s\033[0m@", user);  // Yellow for username
        printf("\033[1;36m%s\033[0m:", hostname);  // Cyan for hostname
        printf("\033[1;32m%s\033[0m", changehome);
        printf(" %s : %ds>\n", cmd_name, elapsed_time); 
        flag_lala=0;
        // printf("haha"); // Green for path
    } else {
        // Print username, hostname, and path in different colors
        printf("\033[1;33m%s\033[0m@", user);  // Yellow for username
        printf("\033[1;36m%s\033[0m:", hostname);  // Cyan for hostname
        printf("\033[1;32m%s\033[0m", home); 
        printf(" %s : %ds>\n", cmd_name, elapsed_time);
        flag_lala=0;
       
        // printf("haha");
    }
    }
}

void shell_prompt() {
    char home[1000];
    getcwd(home, sizeof(home));
    int flag = 0;
    char changehome[1000] = "~";
    int i;

    if (strlen(home_dir) <= strlen(home))
        for (i = 0; home_dir[i] != '\0'; i++) {
            if (home_dir[i] != home[i]) {
                flag = 1;
                break;
            }
        }
    else
        flag = 1;

    if (flag == 0) {
        int j, k;
        for (j = i, k = 1; home[j] != '\0'; j++, k++)
            changehome[k] = home[j];
      printf("\033[1;33m%s\033[0m@", user);  // Yellow for username
        printf("\033[1;36m%s\033[0m:", hostname);  // Cyan for hostname
        printf("\033[1;32m%s\033[0m>", changehome);  // Green for path
    } else {
        // Print username, hostname, and path in different colors
        printf("\033[1;33m%s\033[0m@", user);  // Yellow for username
        printf("\033[1;36m%s\033[0m:", hostname);  // Cyan for hostname
        printf("\033[1;32m%s\033[0m>", home); 
    }
    fflush(stdout);
}

void change_directory(char *path) {
    
    char cwd[MAX_COMMAND_LENGTH];
    int u;
    if (strcmp(path, "~") == 0) {
        path = home_dir;
    } else if (strcmp(path, "-") == 0) {
        if (strlen(prev_dir) == 0) {
            printf("No previous directory set.\n");
            return;
        }
        path = prev_dir;
    }
     else if (strcmp(path, ".") == 0) {
        // Current directory, no change needed
        getcwd(cwd, sizeof(cwd));
        printf("Current directory: %s\n", cwd);
        return;
    }
    

   if(path[0]=='~'){
            char newDir[PATH_MAX];
            snprintf(newDir, sizeof(newDir), "%s%s", home_dir, path + 1);
           u= chdir(newDir);
        //     getcwd(cwd, sizeof(cwd));
        // printf("Changed directory to: %s\n", cwd);
        } else { 
            u= chdir(path);
        //      getcwd(cwd, sizeof(cwd));
        // printf("Changed directory to: %s\n", cwd);
           
        }
        // printf("Trying to change to directory: %s\n", path);
    
    if(u==-1){
            printf("No such directory exists!\n");
            return;
           }
           strcpy(prev_dir,prev_dir_1);
            getcwd(prev_dir_1, sizeof(prev_dir_1));
            // printf("%s",prev_dir);
             getcwd(cwd, sizeof(cwd));
    printf("Changed directory to: %s\n", cwd);
}

char *trim_whitespace(char *str) {
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0) return str;

    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0';
    return str;
}
void check_for_io(char *command, int input_fd, int output_fd, char log[MAX_LOG_SIZE][MAX_COMMAND_LENGTH], int *log_count, int background, char **commands)
{
    char original_command[MAX_COMMAND_LENGTH];
    strncpy(original_command, command, MAX_COMMAND_LENGTH - 1);
    
    char *args[MAX_NUM_ARGS];
    char *token = strtok(command, " \t\r\n");
    int i = 0;
    char *input_file = NULL;
    char *output_file = NULL;
    int redirect = 0;
    int append = 0;

    // Parse command arguments and redirections
    while (token != NULL) {
        if (strcmp(token, "<") == 0) {
            input_file = strtok(NULL, " \t\r\n");
        } else if (strcmp(token, ">") == 0) {
            redirect = 1;
            output_file = strtok(NULL, " \t\r\n");
        } else if (strcmp(token, ">>") == 0) {
            append = 1;
            output_file = strtok(NULL, " \t\r\n");
        } else {
            args[i++] = token;
        }
        token = strtok(NULL, " \t\r\n");
    }
    args[i] = NULL;

    // Handle input redirection
    if (input_file != NULL) {
        int fd_in = open(input_file, O_RDONLY);
        if (fd_in < 0) {
            perror("open input file failed");
            exit(EXIT_FAILURE);
        }
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    } else if (input_fd != -1) {
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
    }

    // Handle output redirection
    if (output_file != NULL) {
        int fd_out;
        if (redirect) {
            fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        } else if (append) {
            fd_out = open(output_file, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
        }
        if (fd_out < 0) {
            perror("open output file failed");
            exit(EXIT_FAILURE);
        }
        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);
    } else if (output_fd != -1) {
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
    }

    // Execute the command with the modified I/O redirections
    execute_command(original_command, log, log_count, background, commands);
}

void execute_command(char *command, char log[MAX_LOG_SIZE][MAX_COMMAND_LENGTH], int *log_count,int background, char **commands) {

   char original_command[MAX_COMMAND_LENGTH];
    strncpy(original_command, command, MAX_COMMAND_LENGTH - 1);
    char *args[MAX_NUM_ARGS];
    char *token = strtok(command, " \t\r\n");
    int i = 0;
    
   
  while (token != NULL && i < MAX_NUM_ARGS - 1) { 
        if (strcmp(token, ">") == 0) {
            // redirect_output = 1;
            // output_file = strtok(NULL, " \t\r\n");
            break;
        } else if (strcmp(token, ">>") == 0) {
            // append = 1;
            // output_file = strtok(NULL, " \t\r\n");
            break;
        } else if (strcmp(token, "<") == 0) {
            // redirect_input = 1;
            // input_file = strtok(NULL, " \t\r\n");
            break;
        }
        args[i++] = token;
        // printf("%s\n",args[i-1]);
        token = strtok(NULL, " \t\r\n");
    }
    args[i] = NULL;
    //  if (redirect_input) {
    //         if (input_file) {
    //             input_fp = fopen(input_file, "r");
    //             if (!input_fp) {
    //                 perror("fopen input");
    //                 return;
    //             }
    //             temp_stdin = stdin;
    //             stdin = input_fp;  // Redirect stdin to input file
    //         } else {
    //             printf("Error: No input file specified for redirection.\n");
    //             return;
    //         }
    //     }

        // Handle output redirection
        
    if (args[0] == NULL) {
        return;
    }

    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }
   
    if (strcmp(args[0], "hop") == 0) { char cwd[MAX_COMMAND_LENGTH];
    
    // for (int k=0;k<i;k++)
    //   printf("what is this?%s\n",args[k]);

        hop_command(args, i);
       
        // add_to_log(log, log_count, original_command);
        // save_log(log, *log_count);
        return;
    }
     if (strcmp(args[0], "proclore") == 0) {
        // Handle proclore command
        
      
        if (args[1] != NULL) {
            proclore_command(args[1]); // With PID
        } else {
            proclore_command(NULL); // No PID, use current process
        }
       
        //  add_to_log(log, log_count, original_command);
        // save_log(log, *log_count);
        return;
    }
    if (strcmp(args[0], "seek") == 0)
    { 
        int file=0;
                int dir=0;
                int eflag=0;
              FILE *temp_stdout = NULL, *temp_stdin = NULL;
    
       int j=1;
        while(args[j][0] == '-') 
        {  
            if(strcmp(args[j],"-f")==0){ 
                        file=1;
                     }
                     else if(strcmp(args[j],"-d")==0){
                        dir=1;
                     }
                      else if(strcmp(args[j],"-e")==0){
                           eflag=1;
                     }
                     j++;
        }
        char search[256]; // Allocate enough memory based on expected input size
char search2[256];
 char directory[2000];
        // printf("%d %d\n",j,i);
        strcpy(search,args[j]);
        // printf("what");
        j++;
        if ((j)<i)
        {
strcpy(search2,args[j]);
        }
        else{strcpy(search2,"NULL");}
        if (strcmp(search2, "NULL") == 0) {
            if (getcwd(directory, sizeof(directory)) != NULL) {
                // `directory` is set to current working directory
            } else {
                perror("getcwd");
                return ;
            }
        } else {
            // Append `search2` to `home_dir`
            snprintf(directory, sizeof(directory), "%s/%s", home_dir, search2);
        }
        // printf("%s",directory);
        seek(search,directory,file,dir,eflag);
        // printf("%s",search);
        // add_to_log(log, log_count, original_command);
        // save_log(log, *log_count);
        
        return;

    }
    if (strcmp(args[0], "iMan") == 0)
    {
        char path[512];
    snprintf(path, sizeof(path), "/?topic=%s&section=all", args[1]);
       
    fetchContent("65.19.140.5", path);

        return ;

    }

if (strcmp(args[0], "reveal") == 0) { 
    char flags[10] = "";  
    char *path = ".";     

   

    for (int j = 1; j < i; j++) {
        if (args[j][0] == '-') {
            if (strcmp(args[j], "-") == 0) {
                if (strlen(prev_dir) > 0) {
                    path = prev_dir;
                } else {
                    fprintf(stderr, "No previous directory stored.\n");
                    return;
                }
            } else {
                // Append individual flags from the argument
                for (int k = 1; args[j][k] != '\0'; k++) {
                    if (strchr(flags, args[j][k]) == NULL) {
                        // Only add the flag if it hasn't been added yet
                        strncat(flags, &args[j][k], 1);
                    }
                }
            }
        } else {
            // If it's not a flag, assume it's a path
            path = args[j];
        }
    }

    reveal(flags, path);



    return;
}

    if(strcmp(args[0],"neonate")==0){
                //    token=strtok(NULL, " \t");
                //    token=(strtok(NULL, " \t"));
                   int time_arg=atoi(args[2]);
                   setRawMode();
    printf("Press 'x' to exit\n");

    while (running) {

        if (isKeyPressed()) {
            char key;
            if (read(STDIN_FILENO, &key, 1) == -1) 
            handleError("read");
            if (key == 'x') {
                running = 0;
                break; // Exit the loop when 'x' is pressed
            }
        }
        pid_t pid = get_recent_pid();
        printf("%d\n", pid);

        sleep(time_arg);
    }

    resetTerminalMode(); // Ensure raw mode is disabled before exiting
    running =1;
    return;
            }

    if (strcmp(args[0], "log") == 0) {
        if (i == 2 && strcmp(args[1], "purge") == 0) {
            purge_log(log, log_count);
            printf("Command log purged.\n");
        } else if (i == 3 && strcmp(args[1], "execute") == 0) {
            int index = atoi(args[2]);
            if (index > 0 && index <= *log_count) {
                index = *log_count - index;
                char log_command[MAX_COMMAND_LENGTH];
                strncpy(log_command, log[index], MAX_COMMAND_LENGTH - 1);
                log_command[MAX_COMMAND_LENGTH - 1] = '\0';

                printf("Executing command from log: %s\n", log_command);
                tokenize_and_execute(log_command,log,log_count,commands);
            } else {
                printf("Invalid index.\n");
            }
        } else {
            print_log(log, *log_count);
        }
        return;
    }

    // if (strstr(original_command, "log") == NULL) {
    //     add_to_log(log, log_count, original_command);
    //     save_log(log, *log_count);
    // }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
    } else if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            printf("Wrong command\n");
        }
        exit(EXIT_FAILURE);
    } else {
        if(background)
        {
            add_background_process(command, pid);
            printf("%d\n",pid);}
        else{check_foreground_process(pid,args[0]);}
        
    }
}
