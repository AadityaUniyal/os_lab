//Write a C program to open a file in the child process, write all even numbers up to n, and then pass the name of the file through a pipe to the parent process. The parent should read the file and print it.

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    int fd[2];
    pid_t pid;
    char filename[] = "even.txt";
    char receivedFile[100];
    int n, num;

    printf("Enter n: ");
    scanf("%d", &n);

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

        int file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);

        if (file < 0)
        {
            printf("File opening failed\n");
            exit(1);
        }

        for (num = 2; num <= n; num += 2)
        {
            char str[20];
            int len = sprintf(str, "%d\n", num);
            write(file, str, len);
        }

        close(file);

        write(fd[1], filename, sizeof(filename));

        close(fd[1]);

        exit(0);
    }
    else
    {
        close(fd[1]);

        wait(NULL);

        read(fd[0], receivedFile, sizeof(receivedFile));

        close(fd[0]);

        printf("\nFile name received from child: %s\n", receivedFile);
        printf("Even numbers are:\n");

        FILE *file = fopen(receivedFile, "r");

        if (file == NULL)
        {
            printf("File opening failed\n");
            return 1;
        }

        while (fscanf(file, "%d", &num) != EOF)
        {
            printf("%d ", num);
        }

        fclose(file);
    }

    return 0;
}