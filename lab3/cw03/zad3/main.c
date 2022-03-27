//
// Created by martynka on 23.03.2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

char *update(char *path, char *curr){
    char * new = calloc(sizeof(char), 1000);
    sprintf(new, "%s/%s", path, curr);
    return new;
}

void search(char *dir_name, char* chain, char depth) {
    DIR *dir;
    FILE *file;
    struct dirent *dp;
    char *path;

    if(depth > 0) {
        dir = opendir(dir_name);
        while (dp = readdir(dir)) {
            if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue;
            else {
                path = update(dir_name, dp->d_name);
                if (dp->d_type == DT_DIR && fork() == 0) {
                    search(path, chain, depth - 1);
                    exit(0);

                }
                else if (strstr(dp->d_name, ".txt")) {
                    file = fopen(path, "r");
                    char *line = calloc(sizeof(char), 256);
                    while (fgets(line, 256 , file)) {
                        if (strstr(line, chain)) {
                            printf("Ścieżka : %s PID: %d\n", path, getpid());
                        }
                    }
                    free(line);
                }
                free(path);
                wait(NULL);
            }
        }
        closedir(dir);
    }
}

int main(int argc, char* argv[]) {
    char* dir;
    char* chain;
    int depth;

    if (argc != 4){
        printf("Zła liczba argumnetów!!!");
        return -1;
    }
    else{
        dir = argv[1];
        chain = argv[2];
        depth = atoi(argv[3]);
    }

    search(dir, chain, depth);

    return 0;
}

