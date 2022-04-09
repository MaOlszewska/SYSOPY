#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>


int main(int argc, char* argv[]) {
    if(argc != 8){
        printf("Nie podałeś argumentów");
        exit(1);
    }

    mkfifo(argv[1], 0666);

    pid_t pid = fork();
    if(pid == 0){

        execl("./consumer", "./consumer","pipe", argv[2], "10", NULL);
    }

    for(int i = 0; i <= 4; i++) {
        char row[2];
        sprintf(row, "%d", i+1);
        pid = fork();
        if (pid == 0) {
            execl("./producent", "./producent", "pipe", row, argv[i + 3], "10", NULL);
        }
    }
    wait(NULL);

    return 0;
}