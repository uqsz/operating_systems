#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <sys/wait.h>

void process_directory(char *path, char *search_str);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <directory path> <search string>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    process_directory(argv[1], argv[2]);
    return 0;
}

void process_directory(char *path, char *search_str) {
    DIR *dir_stream;
    struct dirent *entry;
    struct stat file_stat;
    char full_path[PATH_MAX];
    pid_t pid;

    if ((dir_stream = opendir(path)) == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir_stream)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(full_path, PATH_MAX, "%s/%s", path, entry->d_name);

        if (lstat(full_path, &file_stat) == -1) {
            perror("lstat");
            continue;
        }

        if (S_ISDIR(file_stat.st_mode)) { // process subdirectory
            pid = fork();
            if (pid == -1) {
                perror("fork");
                continue;
            }
            if (pid == 0) { // child process
                process_directory(full_path, search_str);
                exit(EXIT_SUCCESS);
            }
        } else if (S_ISREG(file_stat.st_mode)) { // regular file
            FILE *file_stream = fopen(full_path, "r");
            if (file_stream == NULL) {
                perror("fopen");
                continue;
            }

            char buffer[PATH_MAX];
            if (fgets(buffer, PATH_MAX, file_stream) != NULL) {
                if (strncmp(buffer, search_str, strlen(search_str)) == 0) {
                    printf("%s %d\n", full_path, getpid());
                }
            }
            fclose(file_stream);
        }
    }

    if (closedir(dir_stream) == -1) {
        perror("closedir");
        exit(EXIT_FAILURE);
    }
    while (wait(NULL)>0);
}