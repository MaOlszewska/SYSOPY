#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include  "../zad1/lib_counting.h"

#include <sys/times.h>
#include <unistd.h>

// zadania do wykonania: 1 - Zliczenie dla plików, 0 - usunięcie bloku o podanym indeksie

long double time_sec(clock_t time){
    return (long double)(time) / sysconf(_SC_CLK_TCK);
}

void print_res(clock_t clock_start, clock_t clock_end, struct tms start_tms, struct tms end_tms){
    printf("real %Lf ", time_sec(clock_end - clock_start));
    printf("user %Lf ", time_sec(end_tms.tms_utime - start_tms.tms_utime));
    printf("sys  %Lf \n", time_sec(end_tms.tms_stime - start_tms.tms_stime));
}

int command(char* task){
    if(strchr(task, ':') - task + 1 == 12){
        return 1;
    }
    else if(strchr(task, ':') - task + 1 == 7){
        return 0;
    }
    else return -1;
}

int main(int argc, char* argv[]) {

    struct tms start_tms;
    struct tms end_tms;
    clock_t clock_start;
    clock_t clock_end;

    //char* files[] = {"ok.txt sysop.txt jestfajnie.txt", "sysop.txt ok.txt","0"};

    struct ArrayOfPointers *array = NULL;
    array = create_array(argc - 1);
    for(int i = 1; i < argc; i++) { // zliczenie dla podancyh plików
        int task = command(argv[i]);
        clock_start = times(&start_tms);

        if (task == 1) {    //ogarnąć dla plikóœ
            char files[256] = "" ;
            ++i;
            while ( command(argv[i]) == -1) {
                strcat(files, argv[i]);
                strcat(files, " ");
                if( i < argc - 1 && command(argv[i + 1]) == -1 ) ++i;
                else break;
            }
            printf("COUNT FILES  ");
            count_files(array, files);
            //show_array(array);
        }

        else if(task == 0){ // usuniecie bloku o zadanym indeksie
            printf("REMOVED BLOCK  ");
            remove_block(array, atoi(argv[++i]) );
        }

        else printf("Wrong command");

        clock_end = times(&end_tms);
        print_res(clock_start, clock_end, start_tms, end_tms);
    }
    remove_array(array);
    clock_end = times(&end_tms);
    printf("TOTAL TIME: ");
    print_res(clock_start, clock_end, start_tms, end_tms);

    return 0;
}