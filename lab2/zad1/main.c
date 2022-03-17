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
    printf("real %Lf ", time_sec(clock_end - clock_start));
    printf("user %Lf ", time_sec(end_tms.tms_utime - start_tms.tms_utime));
    printf("sys  %Lf \n", time_sec(end_tms.tms_stime - start_tms.tms_stime));
}


void lib_copy(char* file1, char* file2){

    FILE* firstfile = fopen(file1, "r");
    FILE* secondfile = fopen(file2, "w");

    char tmpchar;
    while(feof(firstfile) == 0){  // zwraca wartość różną od zera jeśli osiągnie się koniec pliku
        bool flag = true;
        do{
            fread(&tmpchar, sizeof(char),1, firstfile);
            if(isspace(tmpchar)){
                if(strspn(&tmpchar," ' '") && !flag){
                    fwrite(&tmpchar, sizeof(char),1,secondfile);
                }
            }
            else{
                flag = false;
                fwrite(&tmpchar,  sizeof(char),1,secondfile);
            }
        }while(tmpchar != '\n');
        if(!flag && feof(firstfile) == 0) fwrite("\n",  sizeof(char),1,secondfile);
    }

    fclose(firstfile);
    fclose(secondfile);
}


void system_copy(char* file1, char* file2){
    int firstfile = open(file1, O_RDONLY);
    int secondfile = open(file2,  O_RDWR);

    char tmpchar;
    bool end = false;
    while(!end){  // zwraca wartość różną od zera jeśli osiągnie się koniec pliku
        bool flag = true;
        do{
            if(read(firstfile, &tmpchar,sizeof (char )) == 0){ //sprawdzam czy koniec pliku
                end = true;
                break;
            };
            if(isspace(tmpchar)){
                if(strspn(&tmpchar," ' '") && !flag){
                    write(secondfile, &tmpchar,sizeof (char ));
                }
            }
            else{
                flag = false;
                write(secondfile, &tmpchar,sizeof (char ));
            }
        }while(tmpchar != '\n');
        if(!flag) write(secondfile, "\n",sizeof (char ));
    }

    close(firstfile);
    close(secondfile);
}

int main(int argc, char* argv[]) {

    struct tms start_tms;
    struct tms end_tms;
    clock_t clock_start;
    clock_t clock_end;

    char* file1;
    char* file2;

    if(argc != 3){
        file1 = calloc(256,sizeof(char));
        file2 = calloc(256, sizeof(char));
        printf("Zapomniałeś o czymś! Podaj nazwy dwóch plików");
        scanf("%s", file1);
        scanf("%s", file2);
    }
    else{
        file1 = argv[1];
        file2 = argv[2];
    }

    printf("Library functions C: ");
    clock_start = times(&start_tms);
    lib_copy(file1, file2);
    clock_end = times(&end_tms);
    print_res(clock_start, clock_end, start_tms, end_tms);

    printf("System functions: ");
    clock_start = times(&start_tms);
    system_copy(file1, file2);
    clock_end = times(&end_tms);
    print_res(clock_start, clock_end, start_tms, end_tms);
}
