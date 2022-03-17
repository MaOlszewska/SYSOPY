//
// Created by martynka on 17.03.2022.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/times.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <fcntl.h>

long double time_sec(clock_t time){
    return (long double)(time) / sysconf(_SC_CLK_TCK);
}

void print_res(clock_t clock_start, clock_t clock_end, struct tms start_tms, struct tms end_tms){
    printf("\nreal %Lf ", time_sec(clock_end - clock_start));
    printf("user %Lf ", time_sec(end_tms.tms_utime - start_tms.tms_utime));
    printf("sys  %Lf \n", time_sec(end_tms.tms_stime - start_tms.tms_stime));
}

void lib_checking(char* file, char character){
    FILE* firstfile = fopen(file, "r");

    char tmpchar;
    int line_counter = 0;
    int counter = 0;

    while(feof(firstfile) == 0){  // zwraca wartość różną od zera jeśli osiągnie się koniec pliku
        bool flag = false;
        do{
            fread(&tmpchar, sizeof(char),1, firstfile);
            if(tmpchar == character && !flag){
                flag = true;
                line_counter ++;
            }
            if(tmpchar == character){
                counter ++;
            }
        }while(tmpchar != '\n');
    }
    printf("In line:  %d\nIn whole file: %d", line_counter, counter);
    fclose(firstfile);
}

void sys_checking(char* file, char character){
    int firstfile = open(file, O_RDONLY);

    char tmpchar;
    int line_counter = 0;
    int counter = 0;
    bool end = false;

    while(!end){  // zwraca wartość różną od zera jeśli osiągnie się koniec pliku
        bool flag = false;
        do{
            if(read(firstfile, &tmpchar,sizeof (char )) == 0){ //sprawdzam czy koniec pliku
                end = true;
                break;
            };
            if(tmpchar == character && !flag){
                flag = true;
                line_counter ++;
            }
            if(tmpchar == character){
                counter ++;
            }
        }while(tmpchar != '\n');
    }
    printf("\nIn line:  %d\nIn whole file: %d", line_counter, counter);
    close(firstfile);
}
int main(int argc, char* argv[]) {

    struct tms start_tms;
    struct tms end_tms;
    clock_t clock_start;
    clock_t clock_end;

    char* file;
    char* character;

    if(argc != 3){
        file = calloc(256,sizeof(char));
        character = calloc(256, sizeof(char));
        printf("Zapomniałeś o czymś! \n");
        scanf("%s", character);
        scanf("%s", file);
    }
    else{
        file = argv[2];
        character = argv[1][0];
    }

    printf("Library functions C: \n");
    clock_start = times(&start_tms);
    lib_checking(file, character);
    clock_end = times(&end_tms);
    print_res(clock_start, clock_end, start_tms, end_tms);

    printf("\nSystem functions: ");
    clock_start = times(&start_tms);
    sys_checking(file, character);
    clock_end = times(&end_tms);
    print_res(clock_start, clock_end, start_tms, end_tms);

}
