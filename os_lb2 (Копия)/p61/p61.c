#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    int fd;
    pid_t pid;

    fd = open("testfile.txt", O_RDWR | O_CREAT | O_APPEND, 0666);
    if (fd == -1) {
        perror("Ошибка открытия файла");
        exit(1);
    }
    
    write(fd, "Родитель: до fork\n", strlen("Родитель: до fork\n"));

    pid = fork();
    if (pid < 0) {
        perror("Ошибка fork");
        exit(1);
    }
    if (pid == 0) { 
        write(fd, "Потомок: после fork\n", strlen("Потомок: после fork\n"));
        off_t pos = lseek(fd, 0, SEEK_CUR);
        printf("Потомок: текущая позиция в файле = %ld\n", pos);
        execl("/bin/echo", "echo", "Потомок: после exec", ">>", "testfile.txt", (char*)NULL);
        perror("Ошибка exec");
        exit(1);
    } else { 
        wait(NULL);
        write(fd, "Родитель: после fork\n", strlen("Родитель: после fork\n"));
        off_t pos = lseek(fd, 0, SEEK_CUR);
        printf("Родитель: текущая позиция в файле = %ld\n", pos);
        close(fd);
    }

    return 0;
}