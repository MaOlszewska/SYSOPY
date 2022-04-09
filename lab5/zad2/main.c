#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ordered_by_date(){
    FILE * file;
    file = popen("echo | mail | sort -M ", "r");  // -k 3 sortowanie po miesiącu
    pclose(file);
    if(file == NULL){
        printf("Błąd przy utworzeniu potoku!\n");
        exit(1);
    }
    printf("Twoje maile posortowane według daty: \n");

    char line[256];
    while(fgets(line, 256, file) != NULL){
        printf("%s \n", line);
    }

}

void ordered_by_sender(){
    FILE * file;
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
    strcat(command, "echo ");
    strcat(command, content);
    strcat(command, " | mail -s ");
    strcat(command, title);
    strcat(command, " ");
    strcat(command, address);

    printf("Mail został wysłany: \n%s\n", command);
    file = popen(command, "w");
    if(file == NULL){
        printf("Błąd przy utworzeniu potoku! email nie został wysłany \n");
        exit(1);
    }
    pclose(file);
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