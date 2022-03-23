//
// Created by martynka on 23.03.2022.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/times.h>
#include <string.h>

long double time_sec(clock_t time){
    return (long double)(time) / sysconf(_SC_CLK_TCK);
}

void print_res(clock_t clock_start, clock_t clock_end, struct tms start_tms, struct tms end_tms){
    printf("real %Lf ", time_sec(clock_end - clock_start));
    printf("user %Lf ", time_sec(end_tms.tms_utime - start_tms.tms_utime));
    printf("sys  %Lf\n", time_sec(end_tms.tms_stime - start_tms.tms_stime));
}

float f(float x){ return 4/(x*x+1); }

float calculate_integral(float width, int n) {
    pid_t pid;  // identyfikacja procsu, służy do reprezentowania procesów, jest to typ liczby całkowitej
    int intervals = (int)(1 / width);
    int count = intervals / n;
    for (int i = 0; i < n; i++) {
        pid = fork();
        wait(NULL);
        if (pid == 0) {  // 0- preces potomy, printujemy tylko dla procesów potomnych
            float tmp = 0.0;
            for(int j = 1; j <=count; j++) {tmp += f(0 + ((i * count) + j ) * width);}
            FILE *fp;
            char file[10];
            char number[10];
            sprintf(number, "%d", i + 1);
            strcat(file,"w");
            strcat(file,number);
            strcat(file, ".txt");
            fp = fopen(file, "w");
            fprintf(fp, "%f", tmp);
            fclose(fp);
            exit(0);
        }
    }

    float value = 0.0;
    for(int i = 1; i <= n; i++) {
        FILE *fp;
        char* file = calloc(256,sizeof(char));
        char number[10];
        sprintf(number, "%d", i);
        strcat(file, "w");
        strcat(file, number);
        strcat(file, ".txt");
        fp = fopen(file, "r");
        char value_file[100];
        fgets(value_file,100,fp);
        value += atof(value_file);
        free(file);
        fclose(fp);
    }
    return value * width;
}

int main(int argc, char* argv[]) {

    struct tms start_tms;
    struct tms end_tms;
    clock_t clock_start;
    clock_t clock_end;

    int n;
    float width;
    if(argc != 3){
        printf("Nie mam argumentu!!!\n");
    }
    else{
        width = atof(argv[1]);
        n = atoi(argv[2]);
    }
    clock_start = times(&start_tms);

    float result = calculate_integral(width,n);

    clock_end = times(&end_tms);
    print_res(clock_start, clock_end, start_tms, end_tms);

    printf("Wartość obliczonej całki wynosi: %f\n\n", result);

    return 0;
}

