#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include  "lab1/zad1/lib_counting.h"
int main(int argc, char* argv[]) {
     char files[] = {"ok.txt sysop.txt jestfajnie.txt"};
//    int size_files = sizeof( files);
//    char command[]= {"wc "};
//   //temp = fopen("temp.txt","w");
//    strcat(command, files);
//    //strcat(command, " > temp.txt");
//
//    FILE* temp = popen(command, "r");
//    char path[100];
//    while (fgets(path, sizeof(path), temp) != NULL) {
//        printf("%s", path);
//    }
//
//    pclose(temp);
//    int counter = 0;
//    for(int i = 0; i < sizeof(files); i++){
//        if(files[i] == ' ')
//            counter ++;
//    }
//    create_array(10);
//    create_counting_blocks(files);
//    printf("%d\n", count_lines(files));
//    printf("%d\n", ++counter);
    //struct Block *block;
    struct ArrayOfPointers *array;
    //block = create_counting_blocks(files);
    array = create_array(10);
    int index;
    index = count_files(array,files);
    //show_block(array->blocks[index]);
    show_array(array);
    //---------------------------------------//
//    remove_rows(array->blocks[index]);
    remove_array(array);
//    show_block(array->blocks[index]);
    //free(array);
    return 0;
}
