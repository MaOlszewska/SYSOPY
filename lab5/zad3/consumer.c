#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void write_to_file(char* line_to_write, int line_number, char* output_name){
    int count = 1;
    FILE *output = fopen(output_name, "r+");
    char tmp[15];
    while(fgets(tmp, 12, output)){
        if(count == line_number){ // wpisuje na odpowiednie miejsce pliku wynikowego pobraną linie
            fputs(line_to_write, output);
            break;
        }
        count ++;
    }
    fclose(output);

}

int main(int argc, char* argv[]) {
    char* pipe_name = argv[1];
    char* output_name = argv[2];
    int number = atoi(argv[3]);
    FILE *pipe = fopen(pipe_name, "r"); // otwiera potok do czytania
    char tmp[number + 10];
    while(fgets(tmp, number + 10, pipe)){ // pobiera z potoku linie
        printf("%s", tmp);
        char * line = strtok(tmp, "-");
        int number_line = atoi(line);
        line = strtok(NULL, "\n");
        write_to_file(line, number_line, output_name); //zapisuje na odpowiednie miejsce linie pobraną z potoku
    }
    fclose(pipe);
    return 0;
}