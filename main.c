#include "headers.h"
#include "log.h"
#include "commands.h"

char prev_dir[MAX_COMMAND_LENGTH] = "";
char home_dir[MAX_COMMAND_LENGTH] = "";
char hostname[MAX_COMMAND_LENGTH] = "";
char prev_dir_1[MAX_COMMAND_LENGTH]="";
char user[1000];

int main() {
     char command[MAX_COMMAND_LENGTH];
     char *commands[1000];
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

    while (1) { int num_commands = 0;
        shell_prompt();
        // Read the command from the user
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;
        }
        command[strcspn(command, "\n")] = '\0';
        //  printf("Initial command: %s\n", command);
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
        // while (cmd != NULL) {
        //     // Trim whitespace and execute the command
        //     char trimmed_cmd[MAX_COMMAND_LENGTH];
        //     strncpy(trimmed_cmd, cmd, MAX_COMMAND_LENGTH - 1);
        //     trimmed_cmd[MAX_COMMAND_LENGTH - 1] = '\0';
        //     trim_whitespace(trimmed_cmd);
        //     execute_command(trimmed_cmd, log, &log_count);

        //     cmd = strtok(NULL, ";");
        // }
        //  printf("%d\n",num_commands);
    // Iterate through the commands array and execute each command
    for (int i = 0; i < num_commands; i++) {
        // printf("Processing command: '%s'\n", commands[i]);
        execute_command(commands[i],log,&log_count);

        // Free the allocated memory for the command
        free(commands[i]);
    }
       
    }

    return 0;
}
