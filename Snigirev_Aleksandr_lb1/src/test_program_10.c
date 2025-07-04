#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
    }

    FILE *file = fopen(argv[1], "r");
    if(!file){
        perror("Open File error");
        return EXIT_FAILURE;
    }

    char buffer[1024];
    size_t bytesRead;
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }
    fclose(file);
    return 0;
}