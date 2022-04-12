
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

    FILE* pipe = fopen(pipe_name, "w"); // otwiera potok nazwany aby można było w nim zapisywać
    FILE *input = fopen(file_name, "r");

    char tmp[number + 10];
    char line[number + 10];
    while(fgets(tmp, number + 10, input)){
        sprintf(line,"%d-%s",row, tmp);
        strcat(line, "\n");
        fwrite(line, 15,15,pipe); // zapisuje do potoku numer wiersza i odczytany fragment pliku
        int r = rand() % 2;
        sleep(r); // odczekuje losową ilosć czasu 1 lub 2 sekundy
    }
    fclose(pipe); // po odczytaniu całego pliku zamyka potok i plik z którego czytał
    fclose(input);
}