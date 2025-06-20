#include"headers.h"
#include"neonate.h"


void resetTerminalMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios) == -1)
        handleError("tcsetattr");
}
void setRawMode() {
    if (tcgetattr(STDIN_FILENO, &original_termios) == -1)
        handleError("tcgetattr");
    
    atexit(resetTerminalMode); // Ensure reset on exit
    
    struct termios raw = original_termios;
    raw.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        handleError("tcsetattr");
}

int isKeyPressed() {
    struct timeval tv = { 0 };  // Zero timeout
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    return select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) > 0;
}
void handleError(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}
pid_t get_recent_pid() {
    DIR *dir;
    struct dirent *ent;
    pid_t newest_pid = 0;
    time_t newest_time = 0;

    dir = opendir("/proc");
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((ent = readdir(dir)) != NULL) {
        char path[256];
        struct stat statbuf;
        pid_t pid;

        // Skip non-numeric directory entries
        if (!isdigit(ent->d_name[0])) {
            continue;
        }

        pid = atoi(ent->d_name);
        snprintf(path, sizeof(path), "/proc/%d", pid);

        if (stat(path, &statbuf) == 0) {
            if (statbuf.st_ctime > newest_time) {
                newest_time = statbuf.st_ctime;
                newest_pid = pid;
            }
        }
    }

    closedir(dir);
    return newest_pid;
}

void sigint_handler(int sig) {
    // Exit the program when 'x' is pressed
    exit(0);
}