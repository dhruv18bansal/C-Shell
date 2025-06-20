#include"headers.h"
#include"activities.h"

void handle_ping_command(char* command) {
   char *token = strtok(command, " ");
    
    // Get the PID
    pid_t pid = atoi(token);
    
    token = strtok(NULL, " "); // Get the signal number
    int signal_number = atoi(token);

    // Check if the process exists in the list
    int process_found = 0;

    for (int i = 0; i < process_count; i++) {
        if (process_list[i].pid == pid) {
            process_found = 1;

            // Send the signal to the process
            if (kill(pid, signal_number) == 0) {
                signal_sent_by_ping = 1; // Mark that the signal was sent by ping
                printf("Signal %d sent to process %d\n", signal_number, pid);

                // Handle termination signals (SIGKILL, SIGTERM, etc.)
                if (signal_number == SIGKILL || signal_number == SIGTERM) {
                    printf("Process (PID %d) terminated \n", pid);
                    remove_background_process(pid);  // Remove from list
                } 
                // Handle stop signals (SIGTSTP, SIGSTOP, etc.)
                else if (signal_number == SIGTSTP || signal_number == SIGSTOP) {
                    process_list[i].running = 0;  // Mark as stopped
                    printf("Process (PID %d) stopped\n", pid);
                } 
                // Handle continue signals (SIGCONT)
                else if (signal_number == SIGCONT) {
                    process_list[i].running = 1;  // Mark as running
                    printf("Process (PID %d) continued\n", pid);
                }
            } else {
                perror("kill");
            }
            break;
        }
    }

    if (!process_found) {
        printf("No such process found\n");
    }
}