#include "headers.h"
#include "signal.h"
void handle_sigint(int sig) {
    // Handle Ctrl-C (SIGINT)
    if (foreground_pid != -1) {
        kill(foreground_pid, SIGINT);
    }
}
void handle_ctrl_d() {
    // Handle Ctrl-D (EOF)
    // Kill all processes and exit the shell
    for (int i = 0; i < process_count; i++) {
        kill(process_list[i].pid, SIGKILL);
    }
    exit(EXIT_SUCCESS);
}