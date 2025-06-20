#include "headers.h"
#include "fgbg.h"
void handle_bg_command(char *command) {
    pid_t pid = atoi(command);

    int process_found = 0;
    for (int i = 0; i < process_count; i++) {
        if (process_list[i].pid == pid) {
            process_found = 1;
            if (process_list[i].running == 0) {
                // The process is stopped, so continue it
                if (kill(pid, SIGCONT) == 0) {
                    process_list[i].running = 1; // Mark as running
                    printf("Process (PID %d) resumed\n", pid);
                } else {
                    perror("kill");
                }
            } else {
                // The process is already running
                printf("Process (PID %d) is already running\n", pid);
            }
            break;
        }
    }
 if (!process_found) {
        printf("No such process found\n");
    }
}
void handle_fg_command(char *command) {
    pid_t pid = atoi(command);
   
    int process_found = 0;
    for (int i = 0; i < process_count; i++) {
        if (process_list[i].pid == pid) {
            process_found = 1;
            if (process_list[i].running == 0) {
                // The process is stopped, so continue it
                if (kill(pid, SIGCONT) == 0) {
                    process_list[i].running = 1; // Mark as running
                    foreground_pid = pid;
                    waitpid(pid, NULL, 0); // Wait for the process to finish
                    foreground_pid = -1;
                } else {
                    perror("kill");
                }
            } else {
                // The process is already running, just bring it to foreground
                foreground_pid = pid;
                waitpid(pid, NULL, 0); // Wait for the process to finish
                foreground_pid = -1;
            }
            remove_background_process(pid);
            break;
        }
    }

    if (!process_found) {
        printf("No such process found\n");
    }
}
