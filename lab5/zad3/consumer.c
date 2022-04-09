#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void write_to_file(char* line_to_write, int line_number, char* output_name){
    int ch;
    int count = 0;
    FILE *output = fopen(output_name, "w");
    while((ch = fgetc(output)) != EOF){
        if(ch == '\n'){
            count ++;
        }
        if(count == line_number){
            fwrite(line_to_write, 256, 15, output);
            break;
        }
    }
    fclose(output);

}

int main(int argc, char* argv[]) {
    char* pipe_name = argv[1];
    char* output_name = argv[2];
    int number = atoi(argv[3]);
    FILE *pipe = fopen(pipe_name, "r");


    char tmp[number + 10];
    while(fread(tmp, number + 10, number, pipe)){
        printf("%s", tmp);
        char* line = strtok(tmp, "->");
        int line_number = atoi(line);
        line = strtok(NULL, "\n");
        char line_to_write[number + 10];
        sprintf((char *) line_to_write, "%s", line); // to co mam zle w pierwszym zadniu
        write_to_file((char *) line_to_write, line_number, output_name);

    }
    fclose(pipe);
    return 0;
}