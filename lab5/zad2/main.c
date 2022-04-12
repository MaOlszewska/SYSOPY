#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ordered_by_date(){
    FILE * file;  // zapisuje do pliku
    // popen tworzy potok, nowy proces i ustawia jego wyjscie/lub wejscie na odpowiednią koncówke potoku
    file = popen("echo | mail | sort -M ", "r");  // -M sortowanie po miesiącu
    // "r" bo chcemy odczytać wyjscie procesu, zwraca obiekt FILE*
    pclose(file); // pclose oczekuje kiedy proces zakonczy działanie
    if(file == NULL){
        printf("Błąd przy utworzeniu potoku!\n");
        exit(1);
    }
    printf("Twoje maile posortowane według daty: \n");

    char line[256];
    while(fgets(line, 256, file) != NULL){
        printf("%s \n", line);  // wypisuje maile posortowane
    }

}

void ordered_by_sender(){
    FILE * file; // popen tworzy nowe potok, proces i ustawia jego wejscie lub wyjscie na dobrą koncówke potoku
    file = popen("echo | mail | sort -k 3 ", "r");  // -k 3 sortowanie po trzeciej kolumnie czyli po nadawcy
    pclose(file);
    if(file == NULL){
        printf("Błąd przy utworzeniu potoku!\n");
        exit(1);
    }
    char line[256];
    printf("Twoje maile posortowane według nadawcy: \n");
    while(fgets(line, 256, file) != NULL){
        printf("%s \n", line);
    }

}

void send(char* address, char* title, char* content){
    FILE * file;
    char * command = (char*) calloc (256, sizeof(char));
    strcat(command, "echo "); // sklejam komende wysyłającą maila
    strcat(command, content);
    strcat(command, " | mail -s ");
    strcat(command, title);
    strcat(command, " ");
    strcat(command, address);

    printf("Mail został wysłany: \n%s\n", command);
    file = popen(command, "w"); // tworzy się nowy potok, proces i i ustawia jego standardowe wyjscie na dobrą koncówke potoku
    if(file == NULL){  // "w" ozancza, że chcemy pisać na wejsciu procesu
        printf("Błąd przy utworzeniu potoku! email nie został wysłany \n");
        exit(1);
    }
    pclose(file);  // oczekuje na zakonczenie działania procesu
    free(command);

}

int main(int argc, char* argv[]) {
    // jeden argument - nadawca lub data
    if(argc == 2 ){ // jeślei jest jedan argument to wypisuje listę posortowaną alfabetycznie według nazwy lub daty
        if(strcmp(argv[1], "data") == 0){
            ordered_by_date();
        }
        else if(strcmp(argv[1],"nadawca") == 0){
            ordered_by_sender();
        }
    }

    if(argc == 4){
        char* address = argv[1];
        char* title = argv[2];
        char* content = argv[3];
        send(address, title, content);
    }
    return 0;
}