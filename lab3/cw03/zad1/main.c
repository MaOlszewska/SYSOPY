//
// Created by martynka on 23.03.2022.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


int main(int argc, char* argv[]) {
    int n;
    if(argc != 2){
        printf("Nie mam argumentu!!!\n");
    }
    else{
        n = atoi(argv[1]);
    }
    pid_t pid;  // identyfikacja procsu, służy do reprezentowania procesów, jest to typ liczby całkowitej

    for(int i = 0; i < n; i++){
        pid = fork();
        wait(NULL);
        if (pid == 0){  // 0- preces potomy, printujemy tylko dla procesów potomnych
            printf("Jestem z procesu: %d\nMoim rodzicem jest: %d\nMoim dzieckiem jest: %d\n\n", i + 1, getppid(), getpid());
            exit(0);
        }
    }
    return 0;
}

