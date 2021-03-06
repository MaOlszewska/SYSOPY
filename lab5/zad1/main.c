#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>


int get_number(char *command){ // funkcja w której zwracam numer komendy
    if(strcmp(command, "command0") == 0) return 0;
    else if(strcmp(command, "command1") == 0) return 1;
    else if(strcmp(command, "command2") == 0) return 2;
    else if(strcmp(command, "command3") == 0) return 3;
    else return 4;
}

int * get_number_of_commands(char*command){   // zwracam numery komend, mtóre skłądają sie na konkretne polecnie
    int * number =  calloc(10, sizeof (int));
    char *tmp =strtok(command, "|");
    int counter = 0;
    while(tmp != NULL){
        number[counter] = get_number(tmp);
        tmp = strtok(NULL, "|");
        counter++;
    }
    number[counter] = NULL;
    return  number;

}

char** get_number_commands(char* command, char** commands){ // zwracam sklejone komendy
    char **lines_num = (char **)calloc(256, sizeof(char *));
    int counter = 0;
    int number = 0;
    int *numbers = get_number_of_commands(command);
    for(int i = 0; i < 2; i++){
        char *tmp = strtok(commands[numbers[i]], "=");
        tmp = strtok(NULL, "|");
        while(tmp != NULL){
            lines_num[counter] = tmp;
            tmp = strtok(NULL, "|");
            counter++;
        }

    }
    lines_num[counter] = NULL;
    return lines_num;

}


void execute_commands(FILE* file){
    char *command = (char *) calloc(256, sizeof(char));
    int count_commands;
    char **commands = (char **) calloc(10, sizeof(char *));
    char **lines_num;
    int counter = 0;
    while (fgets(command, 256 * sizeof(char), file)) {  // przechodzę po wszytskich wierszach pliku
        if (strstr(command, "=")) {                     // komendy zapisuje do tablicy
            char *tmp_line = (char *) calloc(256, sizeof(char));    // a później sklejam z nich polecenia do wykonania
            strcpy(tmp_line, command);
            commands[counter] = tmp_line;
            counter++;

        }
        else{
            printf("Wykonuje linie: %s\n", command);
            count_commands = 0;
            lines_num =  get_number_commands(command, commands);
            for(int i = 0; i < 10; i++){
                if(lines_num[i] != NULL){  //licze ile mam polceń do wykonania
                    count_commands ++;
                }
            }
            int curFd[2]; // tablice z dwoma liczbami całkowitymi, do których zapisawyne są po stworzeniu potoku
            int prevFd[2]; // deskryptory  [0]- odczyt [1]- zapis
            // przekierowuje standradowe wyjscie procesu do koljengo
            for(int i = 0; i < count_commands; i++){  // każde polecenie wykonuje w innym procesie
                pipe(curFd);
                pid_t pid = fork();
                if(pid == 0){  // ustawiamy w stworzonym porocesie wejscie/wyjscie na odpowiednie deskryptory potoku
                    if(i > 0){  // jej działanie polega na skopowiania deskryptora i ustawieniu w storzonym procesie
                        dup2(prevFd[0], STDIN_FILENO);
                    }
                    if(i < count_commands - 1){
                        dup2(curFd[1], STDOUT_FILENO);
                    }
                    if( execvp(lines_num[0], lines_num) == -1){  // wywołuje polecenie
                        exit(1);
                    }
                }
                else
                {
                    if( i > 0){
                        close(prevFd[0]);
                        close(prevFd[1]);
                    }
                    if(i !=  count_commands -1){
                        prevFd[0] = curFd[0];
                        prevFd[1] = curFd[1];
                    }
                }
                wait(NULL);
            }
        }

    }
}

int main(int argc, char* argv[]) {
    FILE *file = fopen(argv[1],"r"); // otwieram plik z poleceniami, tylko do odczytu
    execute_commands(file);
    fclose(file);
    return 0;
}