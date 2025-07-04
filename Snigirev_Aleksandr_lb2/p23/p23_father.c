#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    setenv("MY_VAR", "Hello from execl", 1);

    char *args[] = {"./son", "param1", "param2", NULL};
    char *env[] = {"MY_VAR=Hello from execle", NULL};
    printf("Родительский процесс: PID = %d\n", getpid());
    execl("./son", "son", "param1", "param2", NULL);
    //execvp(args[0], args);
    //execle("./son", "son", "param1", "param2", NULL, env);

    perror("execvp failed");
    return 1;
}