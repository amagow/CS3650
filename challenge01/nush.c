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
#include "ast.h"
#include "parse.h"

void pwait_fork(char *cmd, char *const args[])
{
    int cpid;
    int status;
    if ((cpid = fork()))
    {
        waitpid(cpid, &status, 0);
    }
    else
    {
        execvp(cmd, args);
    }
}

void eval(char *const args[], int size)
{
    char *targ[size + 1];
    for (size_t i = 0; i < size; i++)
    {
        targ[i] = args[i];
    }
    targ[size] = NULL;

    pwait_fork(targ[0], targ);
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
    eval(args->data, args->size);
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
            read_line(cmd, 250, stdin);
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
            read_line(cmd, 250, fstream);
            execute(cmd);
        }
        fclose(fstream);
    }

    return 0;

    // ast *tast = make_ast_value(0);   
    // return 0;

}
