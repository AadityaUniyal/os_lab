//Write a c program to demonstrate zombie process and orphan process through pipe process

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int fd[2];
    pid_t pid;
    char message[100];

    pipe(fd);

    pid = fork();

    if (pid < 0)
    {
        printf("Fork failed\n");
        return 1;
    }

    if (pid == 0)
    {
        close(fd[0]);

        write(fd[1], "Child process is terminating", 28);

        close(fd[1]);

        printf("Child process exiting. PID = %d\n", getpid());

        exit(0);
    }
    else
    {
        close(fd[1]);

        read(fd[0], message, sizeof(message));
        message[28] = '\0';

        printf("Parent received: %s\n", message);

        printf("Parent PID = %d\n", getpid());
        printf("Child PID = %d\n", pid);

        printf("Parent sleeping. Child becomes zombie temporarily.\n");

        sleep(10);

        wait(NULL);

        printf("Parent collected child using wait().\n");
        printf("Zombie removed.\n");

        close(fd[0]);
    }
    return 0;
}