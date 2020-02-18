#ifndef AST_H
#define AST_H

#include "svec.h"

typedef struct shell_ast
{
    //Command string
    char *cmnd;

    struct shell_ast *larg;
    struct shell_ast *rarg;

    //String array of arguments
    svec *args;
} shell_ast;

shell_ast *make_ast_value(svec *args);
shell_ast *make_ast_op(const char* op, shell_ast *a0, shell_ast *a1);
void free_ast(shell_ast *ast);
int ast_eval(shell_ast *ast);
void print_ast(shell_ast *ast);

#endif