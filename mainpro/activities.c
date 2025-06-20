#include "headers.h"
#include"activities.h"
int compare_processes(const void *a, const void *b) {
    return ((Process *)a)->pid - ((Process *)b)->pid;
}
void print_background_process_list() {
    if (process_count == 0) {
        printf("No processes to display.\n");
        return;
    }
 qsort(process_list, process_count, sizeof(Process), compare_processes);
    // printf("Command Name\tPID\tState\n");
    for (int i = 0; i < process_count; i++) {
        printf("%d:\t%s\t-%s\n", process_list[i].pid, process_list[i].command_name,
               process_list[i].running ? "Running" : "Stopped");
    }
}
void remove_background_process(pid_t pid) { 
            

    for (int i = 0; i < process_count; i++) {
        if (process_list[i].pid == pid) {
            // Shift remaining processes up
            for (int j = i; j < process_count - 1; j++) {
                process_list[j] = process_list[j + 1];
            }
            process_count--;
          
            break;
        }
    }
}
void add_background_process(char *command, pid_t pid) {
    if (process_count < MAX_PROCESSES) {
        strcpy(process_list[process_count].command_name, command);
        process_list[process_count].pid = pid;
        process_list[process_count].running = 1; // Initially running
        process_count++;
    } else {
        printf("Background process list is full!\n");
    }
}