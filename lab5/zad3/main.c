#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>


int main(int argc, char* argv[]) {
    if(argc != 8){
        printf("Nie podałeś argumentów");
        exit(1);
    }

    mkfifo(argv[1], 0666);

    pid_t pid = fork();
    if(pid > 0){
        // argv[1] - fifo , argv[2] - scieżka outputu, 10 - liczba znaków odczytywanych jednorazowo
        execl("./consumer", "./consumer","pipe", argv[2], "5", NULL);
    }

    for(int i = 0; i <= 4; i++) {
        char row[2];
        sprintf(row, "%d", i);
        pid = fork();
        if (pid > 0) {
            // argv[1] - fifo , argv[i + 3] - scieżka outputu, 10 - liczba znaków odczytywanych jednorazowo
            execl("./producent", "./producent", "pipe", row, argv[i + 3], "5", NULL);
        }
    }
    return 0;
}