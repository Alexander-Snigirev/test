#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

#define FIFO_PATH "/tmp/myfifo"

int main() {
    struct stat fifo_stat;

    // Проверяем существование и характеристики FIFO
    if (stat(FIFO_PATH, &fifo_stat) == -1) {
        perror("stat failed");
        exit(EXIT_FAILURE);
    }

    printf("FIFO Analysis:\n");
    printf("  Path: %s\n", FIFO_PATH);
    printf("  Type: %s\n", S_ISFIFO(fifo_stat.st_mode) ? "FIFO" : "Not a FIFO");
    printf("  Permissions: %o\n", fifo_stat.st_mode & 0777);
    printf("  Size: %ld bytes\n", fifo_stat.st_size);
    printf("  Inode: %ld\n", fifo_stat.st_ino);
    printf("  UID: %d, GID: %d\n", fifo_stat.st_uid, fifo_stat.st_gid);
    printf("  Last modified: %s", ctime(&fifo_stat.st_mtime));

    // Пытаемся открыть и прочитать FIFO
    int fd = open(FIFO_PATH, O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        perror("open FIFO failed");
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
    if (bytes_read == -1 && errno == EAGAIN) {
        printf("  Content: No data available (FIFO is empty or no writer)\n");
    } else if (bytes_read == 0) {
        printf("  Content: FIFO closed by writer\n");
    } else if (bytes_read > 0) {
        printf("  Content: Read %zd bytes: '%s'\n", bytes_read, buffer);
    } else {
        perror("read failed");
    }

    // Закрываем дескриптор
    if (close(fd) == -1) {
        perror("close failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}