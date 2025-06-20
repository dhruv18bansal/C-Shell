#include "headers.h"
#include "log.h"

#define LOG_FILE "command_log.txt"
#define AA 2000
void load_log(char log[MAX_LOG_SIZE][MAX_COMMAND_LENGTH], int *log_count) {
   char log_file_path[AA];
    sprintf(log_file_path, "%s/%s", home_dir, LOG_FILE);
    FILE *file = fopen(log_file_path, "r");
    if (file) {
        char line[MAX_COMMAND_LENGTH];
        *log_count = 0;
        while (fgets(line, sizeof(line), file) != NULL && *log_count < MAX_LOG_SIZE) {
            line[strcspn(line, "\n")] = 0; // Remove newline
            strcpy(log[*log_count], line);
            (*log_count)++;
        }
        fclose(file);
    }
}

void save_log(char log[MAX_LOG_SIZE][MAX_COMMAND_LENGTH], int log_count) {
    char log_file_path[AA];
    sprintf(log_file_path, "%s/%s", home_dir, LOG_FILE);
    FILE *file = fopen(log_file_path, "w");
    if (file) {
        for (int i = 0; i < log_count; i++) {
            fprintf(file, "%s\n", log[i]);
        }
        fclose(file);
    }
}

void print_log(char log[MAX_LOG_SIZE][MAX_COMMAND_LENGTH], int log_count) {
    for (int i = 0; i < log_count; i++) {
        printf("%d %s\n", i + 1, log[i]);
    }
}

void add_to_log(char log[MAX_LOG_SIZE][MAX_COMMAND_LENGTH], int *log_count, const char *command) {
    if (*log_count > 0 && strcmp(log[*log_count - 1], command) == 0) {
        return;
    }

    if (*log_count == MAX_LOG_SIZE) {
        for (int i = 1; i < MAX_LOG_SIZE; i++) {
            strcpy(log[i - 1], log[i]);
        }
        strcpy(log[MAX_LOG_SIZE - 1], command);
    } else {
        strcpy(log[*log_count], command);
        (*log_count)++;
    }
}

void purge_log(char log[MAX_LOG_SIZE][MAX_COMMAND_LENGTH], int *log_count) {
     char log_file_path[AA];
    sprintf(log_file_path, "%s/%s", home_dir, LOG_FILE);
    remove(log_file_path);
    *log_count = 0;
    remove(log_file_path);
}
