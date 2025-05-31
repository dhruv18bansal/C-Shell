#include "reveal.h"
#include "headers.h"
void print_permissions(mode_t mode) {
    printf((S_ISDIR(mode)) ? "d" : "-");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}
void reveal(const char *flags, const char *path) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    char full_path[MAX_COMMAND_LENGTH];
    int show_hidden = 0, long_listing = 0;

    // Parse flags: any combination of -a and -l should be handled
    for (int i = 0; flags[i] != '\0'; i++) {
        if (flags[i] == 'a') {
            show_hidden = 1;
        }
        if (flags[i] == 'l') {
            long_listing = 1;
        }
    }

    if (strcmp(path, "~") == 0) {
        path = home_dir;
    } else if (strcmp(path, "-") == 0) {
        if (strlen(prev_dir) == 0) {
            printf("No previous directory set.\n");
            return;
        }
        path = prev_dir;
    }

    // Open the directory
    if ((dir = opendir(path)) == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (!show_hidden && entry->d_name[0] == '.') {
            continue; // Skip hidden files if -a flag is not set
        }

        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        if (stat(full_path, &file_stat) == -1) {
            perror("stat");
            continue;
        }
  if (S_ISDIR(file_stat.st_mode)) {
            printf("\033[1;34m");  // Blue for directories
        } else {
            printf("\033[1;32m");  // Green for files
        }
        if (long_listing) {
            print_permissions(file_stat.st_mode);
            printf(" %ld %s %s %5ld %s %s\n",
                   (long)file_stat.st_nlink,
                   getpwuid(file_stat.st_uid)->pw_name,
                   getgrgid(file_stat.st_gid)->gr_name,
                   (long)file_stat.st_size,
                   strtok(ctime(&file_stat.st_mtime), "\n"), // Remove newline
                   entry->d_name);
        } else {
            printf("%s\n", entry->d_name);
        }
         printf("\033[0m");
    }

    closedir(dir);
}
