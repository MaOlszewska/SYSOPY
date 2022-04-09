#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
 // TODO jest gitówa chyba raczej

int main(int argc, char* argv[]) {
    char* pipe_name = argv[1];  // scieżka do potoku nazwanego
    char* file_name = argv[3]; // scieżka do pliku tekstoewego
    int row = atoi(argv[2]); // numer wiersza
    int number = atoi(argv[4]); // ilosć znaków odczytywanych

    FILE *pipe = fopen(pipe_name, "w");
    FILE *input = fopen(file_name, "r");

    char tmp[number + 10];
    char line[number + 10];
    while(fgets(tmp, number + 10, input)){
        printf("%s\n", tmp);
        sprintf(line,"%d->%s",row, tmp);
        strcat(line, "\n");
        write(pipe, line, strlen(line)*sizeof(char));
        int r = rand() % 3;
        sleep(r);
    }
    fclose(pipe);
    fclose(input);
}