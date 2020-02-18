#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "svec.h"

shell_ast *make_shell_ast_value(svec *args)
{
    shell_ast* shell_ast = malloc(sizeof(shell_ast));
    shell_ast->cmnd = strdup("=");
    shell_ast->larg = 0;
    shell_ast->rarg = 0;
    shell_ast->args = args;
    return shell_ast;
}

shell_ast *make_shell_ast_op(char *op, shell_ast *a0, shell_ast *a1)
{
    shell_ast *shell_ast = malloc(sizeof(shell_ast));
    shell_ast->cmnd = strdup(op);
    shell_ast->larg = a1;
    shell_ast->rarg = a0;
    shell_ast->args = 0;
    return shell_ast;
}

void free_shell_ast(shell_ast *shell_ast)
{
    if (shell_ast->larg)
    {
        free_shell_ast(shell_ast->larg);
    }
    if (shell_ast->rarg)
    {
        free_shell_ast(shell_ast->rarg);
    }
    free(shell_ast->cmnd);
    free_svec(shell_ast->args);
    free(shell_ast);
}

int shell_ast_eval(shell_ast *shell_ast)
{
    return 5;
}

char *
shell_ast_string(shell_ast *shell_ast)
{
    if (!strcmp(shell_ast->cmnd, "="))
    {
        char *tmp = malloc(256 * sizeof(char)), *pos = tmp;
        size_t tmp_len = 256;
        size_t tmp_size = 0;
        for (size_t i = 0; i < shell_ast->args->size; i++)
        {
            if (tmp_size >= tmp_len)
            {
                tmp = realloc(tmp, 256 * sizeof(char));
                tmp_len += 256;
            }
            tmp_size += sprintf(pos, "%s", shell_ast->args->data[i]);
            pos += tmp_size;
        }
        return tmp;
    }
    else
    {
        char *aa = shell_ast_string(shell_ast->larg);
        char *bb = shell_ast_string(shell_ast->rarg);
        char *cc = malloc(128);
        sprintf(cc, "(%s %s %s)", aa, shell_ast->cmnd, bb);
        free(aa);
        free(bb);
        return cc;
    }
}

void print_shell_ast(shell_ast *shell_ast)
{
    char *text = shell_ast_string(shell_ast);
    printf("%s\n", text);
    free(text);
}