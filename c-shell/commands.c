#include "headers.h"
#include "commands.h"
#include "log.h"
#include "hop.h"
#include "reveal.h"
#include "proclore.h"
#include "seek.h"
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
}

void change_directory(char *path) {
    char cwd[MAX_COMMAND_LENGTH];
   int u;
    if (strcmp(path, "~") == 0) {
        path = home_dir;
    } else if (strcmp(path, "-") == 0) 
    {
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
    // getcwd(prev_dir, sizeof(prev_dir));

    // if (chdir(path) == 0) {
    //     getcwd(cwd, sizeof(cwd));
    //     printf("Changed directory to: %s\n", cwd);
    // } else {
    //     perror("chdir");
    // }
    if(path[0]=='~'){
            char newDir[PATH_MAX];
            snprintf(newDir, sizeof(newDir), "%s%s", home_dir, path + 1);
           u= chdir(newDir);
        } else {
            u= chdir(path);
           
        }
    
    if(u==-1){
            printf("No such directory exists!\n");
            return;
           }
           strcpy(prev_dir,prev_dir_1);
            getcwd(prev_dir_1, sizeof(prev_dir_1));
}

char *trim_whitespace(char *str) {
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0) return str;

    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0';
    return str;
}

void execute_command(char *command, char log[MAX_LOG_SIZE][MAX_COMMAND_LENGTH], int *log_count) {
    char original_command[MAX_COMMAND_LENGTH];
    strncpy(original_command, command, MAX_COMMAND_LENGTH - 1);

    char *args[MAX_NUM_ARGS];
    char *token = strtok(command, " \t\r\n");
    int i = 0;

    while (token != NULL && i < MAX_NUM_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " \t\r\n");
    }
    args[i] = NULL;

    if (args[0] == NULL) {
        return;
    }

    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }

    if (strcmp(args[0], "hop") == 0) {
        hop_command(args, i);
        add_to_log(log, log_count, original_command);
        save_log(log, *log_count);
        return;
    }
     if (strcmp(args[0], "proclore") == 0) {
        // Handle proclore command
        if (args[1] != NULL) {
            proclore_command(args[1]); // With PID
        } else {
            proclore_command(NULL); // No PID, use current process
        }
         add_to_log(log, log_count, original_command);
        save_log(log, *log_count);
        return;
    }
    if (strcmp(args[0], "seek") == 0)
    { 
        int file=0;
                int dir=0;
                int eflag=0;
                
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
        add_to_log(log, log_count, original_command);
        save_log(log, *log_count);
        return;

    }
    if (strcmp(args[0], "reveal") == 0) {
    // Reveal command
    char flags[10] = "";  // Array to store all flags combined
    char *path = ".";     // Default path is the current directory

    for (int j = 1; j < i; j++) {
        if (args[j][0] == '-') {
            // Append all individual flags from the argument
            for (int k = 1; args[j][k] != '\0'; k++) {
                if (strchr(flags, args[j][k]) == NULL) {
                    // Only add the flag if it hasn't been added yet
                    strncat(flags, &args[j][k], 1);
                }
            }
        } else {
            // If it's not a flag, assume it's a path
            path = args[j];
        }
    }

    // Call reveal with the combined flags and the path
    reveal(flags, path);

    add_to_log(log, log_count, original_command);
    save_log(log, *log_count);
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
                execute_command(log_command, log, log_count);
            } else {
                printf("Invalid index.\n");
            }
        } else {
            print_log(log, *log_count);
        }
        return;
    }

    if (strstr(original_command, "log") == NULL) {
        add_to_log(log, log_count, original_command);
        save_log(log, *log_count);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
    } else if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("execvp");
        }
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}
