
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

int main(int argc, char* argv[]) {
    char* pipe_name = argv[1];  // scieżka do potoku nazwanego
    char* file_name = argv[3]; // scieżka do pliku tekstoewego
    int row = atoi(argv[2]); // numer wiersza
    int number = atoi(argv[4]); // ilosć znaków odczytywanych

    FILE* pipe = fopen(pipe_name, "w");
    FILE *input = fopen(file_name, "r");

    char tmp[number + 10];
    char line[number + 10];
    while(fgets(tmp, number + 10, input)){
        sprintf(line,"%d-%s",row, tmp);
        strcat(line, "\n");
        fwrite(line, 15,15,pipe);
        int r = rand() % 2;
        sleep(r);
    }
    fclose(pipe);
    fclose(input);
}