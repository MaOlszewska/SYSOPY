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
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <ftw.h>

int fifo_counter= 0;
int unknow_counter =0;
int dir_counter = 1;
int char_dev_counter = 0;
int block_dev_counter = 0;
int link_counter = 0;
int socket_counter = 0;
int file_counter = 0;

long double time_sec(clock_t time){
    return (long double)(time) / sysconf(_SC_CLK_TCK);
}

void print_res(clock_t clock_start, clock_t clock_end, struct tms start_tms, struct tms end_tms){
    printf("\nreal %Lf ", time_sec(clock_end - clock_start));
    printf("user %Lf ", time_sec(end_tms.tms_utime - start_tms.tms_utime));
    printf("sys  %Lf \n", time_sec(end_tms.tms_stime - start_tms.tms_stime));
}
char *path_update(char *path, char *curr){
    char * new = calloc(sizeof(char), 1000);
    sprintf(new, "%s/%s", path, curr);
    return new;
}
void print_inf(){
    printf("Liczba fifo: %d\n", fifo_counter);
    printf("Liczba zwykłych plików: %d\n", file_counter);
    printf("Liczba katalogów: %d\n", dir_counter);
    printf("Liczba urządzeń znakowych: %d\n", char_dev_counter);
    printf("Liczba urządzeń blokowych: %d\n", block_dev_counter);
    printf("Liczba linków symbolicznych: %d\n", link_counter);
    printf("Liczba soketów: %d\n", socket_counter);
    printf("Liczba plików nieznanych: %d\n", unknow_counter);
}

void return_type(int tmp){
    switch (tmp) {
        case 1:
            printf("Rodzaj pliku:FIFO");
            fifo_counter ++;
            break;
        case 0:
            printf("Rodzaj pliku: Nieznany");
            unknow_counter ++;
            break;
        case 2:
            printf("Rodzaj pliku: Urządzenie znakowe");
            char_dev_counter++;
            break;
        case 4:
            printf("Rodzaj pliku: Katalog");
            dir_counter++;
            break;
        case 6:
            printf("Rodzaj pliku: Urządzenie blokowe");
            block_dev_counter++;
            break;
        case 8:
            printf("Rodzaj pliku: Zwykły plik");
            file_counter ++;
            break;
        case 10:
            printf("Rodzaj pliku: Dowiązanie symboliczne");
            link_counter ++;
            break;
        case 12:
            printf("Rodzaj pliku: Soket");
            socket_counter++;
            break;
    }
}

void search_dir_ver1(char* dir_name){
    DIR* dir;
    struct dirent* dp;
    char* path;
    dir = opendir(dir_name);
    while(dp = readdir(dir)){
        struct stat stats;
        if(strcmp(dp->d_name,".") == 0 || strcmp(dp->d_name,"..") == 0) continue;
        else {
            path = path_update(dir_name, dp->d_name);
            if(dp->d_type != DT_DIR){

                if(stat(path,&stats) == 0){
                    struct tm dt;
                    printf("Ścieżka: %s \n", path);
                    printf("Liczba dowiązań: %lu \n", stats.st_nlink);
                    return_type(dp->d_type);
                    printf("\nRozmiar: %ld \n",stats.st_size);
                    dt = *(gmtime(&stats.st_atime));
                    printf("Data dostępu: %d-%d-%d %d:%d \n", dt.tm_mday, dt.tm_mon, dt.tm_year + 1900,
                           dt.tm_hour, dt.tm_min);
                    dt = *(gmtime(&stats.st_mtime));
                    printf("Data modyfiakacji: %d-%d-%d %d:%d\n", dt.tm_mday, dt.tm_mon, dt.tm_year + 1900,
                           dt.tm_hour, dt.tm_min);
                    printf("----------------------------\n");
                }
            }

            if(dp->d_type == DT_DIR){
                dir_counter++;
                search_dir_ver1(path);
            }
            free(path);
        }
    }
    closedir(dir);
}

void return_type_FTW(int tmp){
    switch (tmp) {
        case FTW_DNR:
            printf("Rodzaj pliku: Katalog");
            dir_counter++;
            break;
        case FTW_F:
            printf("Rodzaj pliku: Zwykły plik");
            file_counter ++;
            break;
        case FTW_SL:
            printf("Rodzaj pliku: Dowiązanie symboliczne");
            link_counter ++;
            break;
    }
}

int display(char*name,struct stat* stats, int type){
    struct tm dt;
    if(type == FTW_DNR || type == FTW_D ) {
        ++dir_counter;
    }
    if(type != 1){
        printf("Ścieżka: %s \n", name);
        printf("Liczba dowiązań: %lu \n", stats->st_nlink);
        return_type_FTW(type);
        printf("\nRozmiar: %ld \n",stats->st_size);
        dt = *(gmtime(&stats->st_atime));
        printf("Data dostępu: %d-%d-%d %d:%d \n", dt.tm_mday, dt.tm_mon, dt.tm_year + 1900,
               dt.tm_hour, dt.tm_min);
        dt = *(gmtime(&stats->st_mtime));
        printf("Data modyfiakacji: %d-%d-%d %d:%d\n", dt.tm_mday, dt.tm_mon, dt.tm_year + 1900,
               dt.tm_hour, dt.tm_min);
        printf("----------------------------\n");
    }
    return 0;
}

void search_dir_ver2(char* dir_name){
    while( ftw("dir1", display,20) == 1) printf("DUPA");
}

int main(int argc, char* argv[]) {

    struct tms start_tms;
    struct tms end_tms;
    clock_t clock_start;
    clock_t clock_end;

    char * dir_name;

    if(argc == 2){
        dir_name = argv[1];
    }
    else{
        dir_name = calloc(256,sizeof(char));
        printf("Zapomniałeś czegoś!");
        scanf("%s", dir_name);
    }

    printf("Library functions C: \n");
    clock_start = times(&start_tms);
    search_dir_ver1(dir_name);
    clock_end = times(&end_tms);
    print_res(clock_start, clock_end, start_tms, end_tms);
    print_inf();

    fifo_counter= 0;
    unknow_counter =0;
    dir_counter = 0;
    char_dev_counter = 0;
    block_dev_counter = 0;
    link_counter = 0;
    socket_counter = 0;
    file_counter = 0;

    printf("\nFTW ");
    clock_start = times(&start_tms);
    search_dir_ver2(dir_name);
    clock_end = times(&end_tms);
    print_res(clock_start, clock_end, start_tms, end_tms);
    print_inf();
}