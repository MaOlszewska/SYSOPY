#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include  "../zad1/lib_counting.h"
//#include  "../zad2/lib_counting.c"

// zadania do wykonania: 1 - Zliczenie dla plików, 0 - usunięcie bloku o podanym indeksie

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
    //char* files[] = {"ok.txt sysop.txt jestfajnie.txt", "sysop.txt ok.txt","0"};

    struct ArrayOfPointers *array = NULL;
    array = create_array(argc - 1);
    printf("%d",argc);
    for(int i = 1; i < argc; i++) { // zliczenie dla podancyh plików
        int task = command(argv[i]);
        printf("%d", i);

        if (task == 1) {    //ogarnąć dla plikóœ
            char files[256] = "" ;
            ++i;
            while ( command(argv[i]) == -1) {
                strcat(files, argv[i]);
                strcat(files, " ");
                if( i < argc - 1 && command(argv[i + 1]) == -1 ) ++i;
                else break;
            }
            count_files(array, files);
            show_array(array);
        }

        else if(task == 0){ // usuniecie bloku o zadanym indeksie
            remove_block(array, atoi(argv[++i]) );
            printf("%d", i);
        }
        else printf("Wrong command");


    }
    remove_array(array);

    return 0;
}