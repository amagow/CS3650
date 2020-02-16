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
    int cpid;
    if ((cpid = fork()))
    {
        int status;
        waitpid(cpid, &status, 0);
    }
    else
    {
        char *targ[size + 1];
        for (size_t i = 0; i < size; i++)
        {
            targ[i] = args[i];
        }
        targ[size] = NULL;

        execvp(cmd, targ);
    }
}

void execute(char *cmd)
{
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
    eval(args->data[0], args->data, args->size);
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
            char *line = read_line(cmd, 250, stdin);
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
            char *line = read_line(cmd, 250, fstream);
            cmd = {0};
            chomp(cmd);
            execute(cmd);
        }
        fclose(fstream);
    }

    return 0;
}
