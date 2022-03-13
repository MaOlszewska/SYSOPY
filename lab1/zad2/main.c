#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/times.h>
#include <unistd.h>

#ifndef DYNAMIC
    #include  "../zad1/lib_counting.h"
#endif

#ifdef DYNAMIC
#include <dlfcn.h>
typedef struct Block
{
    int rows_number;
    char **rows;
} Block;

typedef struct ArrayOfPointers
{
    int size;
    int las_idx;
    struct Block** blocks;
} ArrayOfPointers;

struct ArrayOfPointers* (*create_array)(int);
struct Block* (*create_counting_blocks)(char*);
void (*remove_block)(struct ArrayOfPointers*, int);
int(*count_files)(struct ArrayOfPointers*, char*);
void (*remove_array)(struct ArrayOfPointers*);
void (*remove_rows)(struct Block*);
void (*show_block)(struct Block*);
void (*show_array)(struct ArrayOfPointers*);
#endif

long double time_sec(clock_t time){
    return (long double)(time) / sysconf(_SC_CLK_TCK);
}

void print_res(clock_t clock_start, clock_t clock_end, struct tms start_tms, struct tms end_tms){
    printf("real %Lf ", time_sec(clock_end - clock_start));
    printf("user %Lf ", time_sec(end_tms.tms_utime - start_tms.tms_utime));
    printf("sys  %Lf \n", time_sec(end_tms.tms_stime - start_tms.tms_stime));
}

int command(char* task){
    if(strcmp(task, "count_files:") == 0){
        return 1;
    }
    else if(strcmp(task, "remove:") == 0){
        return 0;
    }
    else if(strcmp(task,"create:") == 0){
        return 2;
    }
    else return -1;
}

int main(int argc, char* argv[]) {
    #ifdef DYNAMIC
    void *handle = dlopen("../zad1/lib_counting.so",RTLD_LAZY);
    create_array = (struct ArrayOfPointers* (*) (int)) dlsym (handle, "create_array");
    create_counting_blocks = (struct Block* (*)(char*)) dlsym (handle, "create_counting_blocks");
    remove_block =(void (*) (struct ArrayOfPointers*, int)) dlsym (handle, "remove_block");
    count_files =(int (*) (struct ArrayOfPointers*, char*)) dlsym (handle, "count_files");
    remove_array =(void (*) (struct ArrayOfPointers*)) dlsym (handle, "remove_array");
    remove_rows = (void (*) (struct Block*)) dlsym (handle, "remove_rows");
    show_block = (void(*) (struct Block*)) dlsym (handle, "show_block");
    show_array = (void (*) (struct ArrayOfPointers*)) dlsym (handle, "show_array");
    #endif

    struct tms start_tms;
    struct tms end_tms;
    clock_t clock_start;
    clock_t clock_end;

    struct ArrayOfPointers *array = NULL;

    if(command(argv[1]) == 2){
        printf("CREATE ARRAY ");
        clock_start = times(&start_tms);
        array = create_array(atoi(argv[2]));
        clock_end = times(&end_tms);
        print_res(clock_start, clock_end, start_tms, end_tms);
    }

    for(int i = 3; i < argc; i++) {
        int task = command(argv[i]);

        if (task == 1) {
            clock_start = times(&start_tms);
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
            clock_end = times(&end_tms);
            print_res(clock_start, clock_end, start_tms, end_tms);
        }

        else if(task == 0){
            clock_start = times(&start_tms);
            printf("REMOVED BLOCK  ");
            remove_block(array, atoi(argv[++i]) );
            clock_end = times(&end_tms);
            print_res(clock_start, clock_end, start_tms, end_tms);
        }

        else printf("Wrong command");
    }

    clock_end = times(&end_tms);
    printf("TOTAL TIME: ");
    print_res(clock_start, clock_end, start_tms, end_tms);
    remove_array(array);

    return 0;
}