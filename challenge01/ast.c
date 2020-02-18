#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

ast *make_ast_value(svec *args)
{
    ast *ast = malloc(sizeof(ast));
    ast->cmnd = "=";
    ast->larg = 0;
    ast->rarg = 0;
    ast->args = args;
    return ast;
}

ast *make_ast_op(char *op, ast *a0, ast *a1)
{
    ast *ast = malloc(sizeof(ast));
    ast->cmnd = op;
    ast->larg = a1;
    ast->rarg = a0;
    ast->args = 0;
    return ast;
}

void free_ast(ast *ast)
{
    if (ast->larg)
    {
        free_ast(ast->larg);
    }
    if (ast->rarg)
    {
        free_ast(ast->rarg);
    }
    free(ast);
}

int ast_eval(ast *ast)
{
    return 5;
}

char *
ast_string(ast *ast)
{
    if (!strcmp(ast->cmnd, "="))
    {
        char *tmp = malloc(256 * sizeof(char)), *pos = tmp;
        size_t tmp_len = 256;
        for (size_t i = 0; i < ast->args->size; i++)
        {
            if (pos >= tmp_len)
            {
                realloc(tmp, 256 * sizeof(char));
                tmp_len += 256;
            }
            pos += sprintf(pos, "%s", ast->args[i]);
        }
        return tmp;
    }
    else
    {
        char *aa = ast_string(ast->larg);
        char *bb = ast_string(ast->rarg);
        char *cc = malloc(128);
        sprintf(cc, "(%s %s %s)", aa, ast->cmnd, bb);
        free(aa);
        free(bb);
        return cc;
    }
}

void print_ast(ast *ast)
{
    char *text = ast_string(ast);
    printf("%s\n", text);
    free(text);
}