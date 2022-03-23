//
// Created by martynka on 17.03.2022.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

char *path_update(char *path, char *curr){
    char * new = calloc(sizeof(char), 1000);
    sprintf(new, "%s/%s", path, curr);
    return new;
}

void print_inf(){
    printf("\nLiczba fifo: %d\n", fifo_counter);
    printf("Liczba zwykłych plików: %d\n", file_counter);
    printf("Liczba katalogów: %d\n", dir_counter);
    printf("Liczba urządzeń znakowych: %d\n", char_dev_counter);
    printf("Liczba urządzeń blokowych: %d\n", block_dev_counter);
    printf("Liczba linków symbolicznych: %d\n", link_counter);
    printf("Liczba soketów: %d\n", socket_counter);
    printf("Liczba plików nieznanych: %d\n", unknow_counter);
}

void return_type(int tmp){
    if (tmp == 4) {printf("Rodzaj pliku: Katalog"); fifo_counter++;}
    else if (tmp == 8) {printf("Rodzaj pliku: Zwykły plik"); file_counter++;}
    else if (tmp == 10) {printf("Rodzaj pliku: Dowiązanie symboliczne");link_counter++;}
    else if (tmp == 2) {printf("Rodzaj pliku: Urządzenie znakowe");char_dev_counter++;}
    else if (tmp == 6) {printf("Rodzaj pliku: Urządzenie blokowe");block_dev_counter++;}
    else if (tmp == 1) {printf("Rodzaj pliku:FIFO"); fifo_counter++;}
    else if (tmp == 12) {printf("Rodzaj pliku: Soket"); socket_counter++;}
    else {printf("Rodzaj pliku: Nieznany"); unknow_counter++;}
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

void return_type_FTW(const struct stat* stats){
    uint mode = stats->st_mode;
    if (S_ISDIR(mode)) {printf("Rodzaj pliku: Katalog"); fifo_counter++;}
    else if (S_ISREG(mode)) {printf("Rodzaj pliku: Zwykły plik"); file_counter++;}
    else if (S_ISLNK(mode)) {printf("Rodzaj pliku: Dowiązanie symboliczne");link_counter++;}
    else if (S_ISCHR(mode)) {printf("Rodzaj pliku: Urządzenie znakowe");char_dev_counter++;}
    else if (S_ISBLK(mode)) {printf("Rodzaj pliku: Urządzenie blokowe");block_dev_counter++;}
    else if (S_ISFIFO(mode)) {printf("Rodzaj pliku:FIFO"); fifo_counter++;}
    else if (S_ISSOCK(mode)) {printf("Rodzaj pliku: Soket"); socket_counter++;}
    else {printf("Rodzaj pliku: Nieznany"); unknow_counter++;}
}

int display(const char*name, const struct stat* stats, int type){
    struct tm dt;
    if(type == FTW_DNR || type == FTW_D ) {++dir_counter;}
    if(type != 1){
        printf("Ścieżka: %s \n", name);
        printf("Liczba dowiązań: %lu \n", stats->st_nlink);
        return_type_FTW(stats);
        printf("\nRozmiar: %ld \n",stats->st_size);
        dt = *(gmtime(&stats->st_atime));
        printf("Data dostępu: %d-%d-%d %d:%d \n", dt.tm_mday, dt.tm_mon, dt.tm_year + 1900,dt.tm_hour, dt.tm_min);
        dt = *(gmtime(&stats->st_mtime));
        printf("Data modyfiakacji: %d-%d-%d %d:%d\n", dt.tm_mday, dt.tm_mon, dt.tm_year + 1900,dt.tm_hour, dt.tm_min);
        printf("----------------------------\n");
    }
    return 0;
}

void search_dir_ver2(char* dir_name){
    while( ftw(dir_name, display,20) == 1);
}

int main(int argc, char* argv[]) {
    char * dir_name;

    if(argc == 2){dir_name = argv[1];}
    else{
        dir_name = calloc(256,sizeof(char));
        printf("Zapomniałeś czegoś! Podaj nazwe katalogu");
        scanf("%s", dir_name);
    }

    printf("Library functions C: \n");
    search_dir_ver1(dir_name);
    print_inf();

    fifo_counter= 0;
    unknow_counter =0;
    dir_counter = 0;
    char_dev_counter = 0;
    block_dev_counter = 0;
    link_counter = 0;
    socket_counter = 0;
    file_counter = 0;

    printf("\nFTW:\n");
    search_dir_ver2(dir_name);
    print_inf();
}