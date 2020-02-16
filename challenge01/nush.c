#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "svec.h"
#include "helper.h"
#include "tokenize.h"

void eval(char *cmd, char *const args[], int size)
{

    char *targ[size + 1];
    for (size_t i = 0; i < size; i++)
    {
        targ[i] = args[i];
    }
    targ[size] = NULL;

    execvp(cmd, targ);
}

void execute(char *cmd)
{
    int cpid;
    svec *args = tokenize(cmd);
    if (!strcmp(args->data[0], "exit"))
    {
        exit(0);
    }
    if (!strcmp(args->data[0], "cd"))
    {
        char *targ = args->data[1];
        chdir(targ);
        return;
    }
    if ((cpid = fork()))
    {
        // parent process
        // printf("Parent pid: %d\n", getpid());
        // printf("Parent knows child pid: %d\n", cpid);

        // Child may still be running until we wait.

        int status;
        waitpid(cpid, &status, 0);

        // printf("== executed program complete ==\n");

        // printf("child returned with wait code %d\n", status);
        if (WIFEXITED(status))
        {
            // printf("child exited with exit code (or main returned) %d\n", WEXITSTATUS(status));
        }
    }
    else
    {
        // child process
        // printf("Child pid: %d\n", getpid());
        // printf("Child knows parent pid: %d\n", getppid());

        // The argv array for the child.
        // Terminated by a null pointer.
        // char* args[] = {cmd, "one", 0};

        eval(args->data[0], args->data, args->size);
        // printf("== executed program's output: ==\n");
        // execvp(cmd, args);
        // printf("Can't get here, exec only returns on error.");
    }
    free_svec(args);
}

int main(int argc, char *argv[])
{
    char cmd[256];

    if (argc == 1)
    {
        while (1)
        {
            printf("nush$ ");
            fflush(stdout);
            char *line = read_line(cmd, 256, stdin);
            chomp(cmd);
            execute(cmd);
        }
    }
    else if (argc == 2)
    {
        FILE *fstream = fopen(argv[1], "r");
        fflush(stdout);
        while (1)
        {
            fflush(stdout);
            char *line = read_line(cmd, 256, fstream);
            chomp(cmd);
            execute(cmd);
        }
        fclose(fstream);
    }

    return 0;
}
